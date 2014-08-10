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

#define _CPF_TOKEN_PREFIX "$"

#ifdef _WIN32
/*warning C4129: '$' : unrecognized character escape sequence...*/
#pragma warning( push )
#pragma warning( disable: 4129 )
#else

#endif
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
	"`$", "`r", "`g", "`b", "`y", "`m", "`c", "`w", "`.", "`*"
};

#ifdef _WIN32
#pragma warning( pop )
#else

#endif

const std::initializer_list<char> std_format_specifiers = { 
	'c', 'd', 'e', 'E', 'f', 'F', 'g', 'G', 'i', 'o', 's', 'u', 'x', 'X', 'a', 'A', 'p', 'n'
};

/*
These are characters that terminate a format specifier:

%#x
%.6i
%05.2f
*/
const std::initializer_list<char> extended_format_specifier_terminators = { 
	'd', 'f', 's', 'e', 'o', 'x', 'X', 'i', 'u'
};

/*
These are characters that typically contained in format specifiers:

%.6i
%05.2f
*/
const std::initializer_list<char> intermediate_format_specifers = { 
	'+', '-', '.', '*', '#', 'l' 
};
const std::initializer_list<char> escape_characters = { 
	'\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\', '\"', '\0'
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

_cpf_type::str parse_format_attributes(	_cpf_type::str const &format_str_, 
										_cpf_type::str::size_type search_start_pos_,
										_cpf_type::str::size_type &attrib_end_pos)
{
	auto is_valid_attrib_char = [&](char const &c)
	{
		auto special_attrib_chars = { '.', '*', '#', '?', ',' };
		return	isdigit(c) || /*implies cursor position attribute*/
				std::find(_cpf_std_tokens.begin(), _cpf_std_tokens.end(), _cpf_type::str({ c })) != _cpf_std_tokens.end() ||
				std::find(special_attrib_chars.begin(), special_attrib_chars.end(), c) != special_attrib_chars.end();
	};

	/*the parsed attribute(s) string...*/
	_cpf_type::str attribute_string;
	auto ssp = search_start_pos_;
	char current_char = format_str_[ssp];

	while (is_valid_attrib_char(current_char))
	{
		attribute_string.append({ current_char });
		current_char = format_str_[++ssp];
	};

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

	attrib_end_pos = ssp;
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
		
		auto attibs_str = parse_format_attributes(src_format, token_occurance_pos + 1, attrib_endpos_p1);
		//suffix_pos = _cpf_find(_CPF_TOKEN_SUFFIX, src_format, prefix_pos);
		
		/*if(suffix_pos == src_format.npos)
		{
			throw _cpf_type::error("cpf err: invalid token encountered");
		}*/

		auto next_prefix_pos = _cpf_find(_CPF_TOKEN_PREFIX, src_format, attrib_endpos_p1);
		//auto attibs_str = src_format.substr(prefix_pos + 1, (suffix_pos - 1) - prefix_pos);

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
		meta.insert(std::make_pair(0u, std::make_pair(_cpf_type::str_vec({ "!" }), src_format)));
	}

	purge_meta_esc_sequences(meta);

	return meta;
}