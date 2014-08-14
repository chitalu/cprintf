/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

*/

#include "cprintf/internal/_cpf_parse.h"
#include "_cpf_config.h"
#include "_cpf_find.h"
#include <cstdio>
#include <stdlib.h>     /* atoi */
#include <algorithm>
#include <functional> //std::function

#define _CPF_TOKEN_PREFIX "$"

/*
	these apply in situations where a user may wish to write a string 
	of characters where the first is one which may mistakably be 
	interpreted as part of the attribute specified...

	i.e cprintf("$rred")

	in the above case does one wish to apply red to foreground or red to both 
	foregound and background?

	to resolve this issue the following is used:

	cprintf("$r`red");
*/
const std::initializer_list<_cpf_type::str> attribute_escape_sequences = { 
	"`$", "`r", "`g", 
	"`b", "`y", "`m", 
	"`c", "`w", "`.", 
	"`*", "``"
};

const std::initializer_list<char> std_format_specifiers = { 
	'c', 'd', 'e', 
	'E', 'f', 'F', 
	'g', 'G', 'i', 
	'o', 's', 'u', 
	'x', 'X', 'a', 
	'A', 'p', 'n'
};

/*
These are characters that terminate a format specifier:
Characters typically found at the end of a more complex FS

%#x
%.6i
%05.2f
*/
const std::initializer_list<char> extended_format_specifier_terminators = { 
	'd', 'f', 's', 
	'e', 'o', 'x', 
	'X', 'i', 'u'
};

/*
These are characters that typically contained within i.e. in the middle of more 
complex format specifiers:

%.6i
%05.2f
*/
const std::initializer_list<char> intermediate_format_specifers = { 
	'+', '-', '.', 
	'*', '#', 'l' 
};

const std::initializer_list<char> escape_characters = { 
	'\a', '\b', '\f', 
	'\n', '\r', '\t', 
	'\v', '\\', '\"', 
	'\0'
};

auto col_ids = { 
	'r', 'g', 'b', 
	'y', 'm', 'c', 
	'w' 
};

/*characters that may precede colour identifiers (r, g, b...)
and '#' in the case of '*'..*/
auto col_id_prefs = { '.', '*' };

/*specialised character identifiers*/
auto schar_ids = 
{ 
	'!', '~', 'f', 
	'b', '&', '#', 
	'*', '.' 
};

/*
	parse-predicate storage type
*/
typedef std::map<
	char,
	std::function<bool(_cpf_type::str const &, std::size_t const &)>
> ppred_t;

bool pred_isdigit(_cpf_type::str const &s, std::size_t const &p)
{
	return isdigit(s[p]) ? true : false;
}

bool pred_scrn_clr(_cpf_type::str const &s, std::size_t const &p)
{
	return s[p] == '!';
}

bool pred_colour(_cpf_type::str const &s, std::size_t const &p)
{
	/*a colour char may only be preceded by another or any char contained in col_id_prefs...*/
	return	(std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end()) ||
		(std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) != col_id_prefs.end());
}

/*
	map of predicates for conditional syntax checking based on encountered characters
	on parsing.

	the returned value from all predicates signifies whether the encountered
	syntax constitues valid (true) token specification with the format string
	or not (false). Where a return value of false would, in [most] but not all
	cases, result in parsing at a particular position in the format string being 
	recognised as finished.
	By "position" it is meant the position at which a cprintf syntax token ($)
	is encountered, incremented by 1 i.e $r, parsing start position in this case 
	would be at format-string index position of character 'r'.

	note: the argument p accepted by all predicates signifies the positional index
	preceding the character at the current search position. This "character" 
	is always the [key]
*/
const ppred_t parsing_predicates = {
	{ '!', pred_scrn_clr },/*screen wipe + reset position to 0,0 in console*/
	{ '~', pred_scrn_clr },/*screen wipe + retain current position -> !~ -> $!~...*/
	{ ',', pred_isdigit },
	{ 'f', pred_isdigit },
	{ '&', pred_isdigit },
	{ 'r', pred_colour },
	{ 'g', pred_colour },
	/*blue shall use a different predicate*/
	{ 'y', pred_colour },
	{ 'm', pred_colour },
	{ 'c', pred_colour },
	{ 'w', pred_colour },
	{
		'?',
		[&](_cpf_type::str const &s, std::size_t const &p)->bool{ return s[p] == '?'; }
	},
	{
		'*', /*an asterisk can only be prefixed by colour identifiers*/
		[&](_cpf_type::str const &s, std::size_t const &p)->bool
		{
			return std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end(); /*in r, g, b, ...*/;
		}
	},
	{
		'.',
		[&](_cpf_type::str const &s, std::size_t const &p)->bool
		{
			//s[p] in r, g, b, ... or is digit or b f & etc
			return	(std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end()) ||
				isdigit(s[p]) ||
				(std::find(schar_ids.begin(), schar_ids.end(), s[p]) != schar_ids.end());
		}
	},
	{
		'#',
		[&](_cpf_type::str const &s, std::size_t const &p)->bool
		{
			return	(std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end()) ||
					s[p] == '*';
		}
	},
	{
		'b',
		[&](_cpf_type::str const &s, std::size_t const &p)->bool
		{
			/*note: this covers the colour blue (...$b[*]...) as well as a token representing
			a unix bitmap-colour token (...$34b...).
			in effect, what this means is that the blue colour token shall not be treated
			in a similar manner as the rest of the colours for reasons of possible ambiguity
			in parsing...*/
			return	(isdigit(s[p]) ? true : false) ||
				std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end() ||
				(std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) != col_id_prefs.end());
		}
	}
};

/*
	searches "subject" for "search" and replaces it with "replace"
*/
void str_replace(_cpf_type::str& subject, const _cpf_type::str& search, const _cpf_type::str& replace)
{
	size_t pos = 0;
	while ((pos = _cpf_find(search, subject, pos)) != _cpf_type::str::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

void purge_str_esc_sequences(_cpf_type::str &src)
{
	for (auto &es : attribute_escape_sequences)
	{
		str_replace(src, es, es.substr(1));
	}
}

void purge_meta_esc_sequences(_cpf_type::meta_format_type& meta)
{
	for (auto &e : meta)
	{
		purge_str_esc_sequences(e.second.second);
	}
}

void parse(_cpf_type::str const& s_, _cpf_type::str::size_type &offset_pos, _cpf_type::str::size_type &ssp)
{
	std::size_t offset_counter = 0;
	char c = s_[ssp];//first character after occurrance of "$" 
	bool finished = false;
	ppred_t::const_iterator pred_iter;
	while (!finished)
	{
		pred_iter = parsing_predicates.find(c);
		if (pred_iter != parsing_predicates.end())
		{
			do
			{
				auto off = ssp + (++offset_counter);

				/*on first iteration, take away only 1 from "off" since
				character compared to in predicate will not have any other preceding
				char but "$". All other comparisons involve comparing the 'current'
				char with the one preceding its position to verify syntax correction
				*/
				auto x = (offset_counter) == 1 ? 1 : 2;
				/*if character found does abide by parsing rules...*/
				if (pred_iter->second(s_, off - x) == false)
				{
					finished = true;
					break;
				}
				c = s_[off];
			} while ((pred_iter = parsing_predicates.find(c)) != parsing_predicates.end());
		}
		else if (isdigit(c))
		{
			do
			{
				auto off = ssp + (++offset_counter);
				c = s_[off];
			} while (isdigit(c));
		}
		else
		{
			finished = true;
		}
	}

	offset_pos = offset_counter;
}

_cpf_type::str parse_format_attributes(	_cpf_type::str const &format_str_, 
										_cpf_type::str::size_type search_start_pos_,
										_cpf_type::str::size_type &attrib_end_pos)
{
	auto ssp = search_start_pos_;

	/*offset from search start position i.e token occurance position in format string*/
	_cpf_type::str::size_type offset_pos = 0;
	
	parse(format_str_, offset_pos, ssp);

	/*the parsed attribute(s) string...*/
	_cpf_type::str attribute_string = format_str_.substr(ssp, offset_pos);

	if (attribute_string.size() == 0)
	{
		const char* emsg = R"err(
cpf error: 
invalid token encountered @ %d
)err"; 
		char buf[64]; 
#ifdef _WIN32
		sprintf_s(buf, emsg, ssp);
#else
		sprintf(buf, emsg, ssp);
#endif
		throw _cpf_type::error(buf);
	}

	attrib_end_pos += offset_pos;
	return attribute_string;
}

_cpf_type::meta_format_type _cpf_process_format_string(
	const _cpf_type::str &src_format)
{
	_cpf_type::meta_format_type meta;

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_type::str::size_type start = 0;

		while ((start = _cpf_find(_CPF_TOKEN_PREFIX, src_format, start)) != _cpf_type::str::npos)
		{
			++occurrences;
			start++;
		}
		return occurrences;
	}();
	
	std::size_t token_occurance_pos = 0, attrib_endpos_p1 = 0;
	bool first_iter = true;
	while ((token_occurance_pos = _cpf_find(_CPF_TOKEN_PREFIX, src_format, attrib_endpos_p1)) != src_format.npos)
	{
		if (first_iter && token_occurance_pos != 0)
		{
			_cpf_type::str_vec default_attrib{ "?" };
			meta.insert(std::make_pair(0u, std::make_pair(default_attrib, src_format.substr(0u, token_occurance_pos))));
			first_iter = false;
		}
		
		auto endpoint_offset = token_occurance_pos + 1;
		auto attibs_str = parse_format_attributes(src_format, token_occurance_pos + 1, endpoint_offset);
		attrib_endpos_p1 += endpoint_offset;

		auto next_prefix_pos = _cpf_find(_CPF_TOKEN_PREFIX, src_format, attrib_endpos_p1);
	
		_cpf_type::str_vec subseq_str_attribs;
		_cpf_type::str current_attrib;

		for(auto c = std::begin(attibs_str); c != std::end(attibs_str); ++c)
		{
			if(*c == '.')
			{
				subseq_str_attribs.push_back(current_attrib);
				current_attrib.clear();
				continue;
			}
			current_attrib.append({*c});
			//reached last character in string...
			if (std::distance(attibs_str.begin(), c) == (attibs_str.size() - 1) )
			{
				subseq_str_attribs.push_back(current_attrib);
			}
		}

		/*the sub string to which the encountered attributes shall be applied*/
		auto meta_substr = src_format.substr(attrib_endpos_p1, (next_prefix_pos - attrib_endpos_p1));

		meta.insert({ 
			token_occurance_pos, 
			{subseq_str_attribs, meta_substr} 
		});
	}

	if (meta.empty())
	{
		meta.insert(std::make_pair(0u, std::make_pair(_cpf_type::str_vec({ "?" }), src_format)));
	}

	purge_meta_esc_sequences(meta);

	return meta;
}