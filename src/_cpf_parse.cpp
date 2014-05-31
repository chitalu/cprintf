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

#include "_cpf_parse.h"
#include "_cpf_config.h"
#include "_cpf_find.h"
#include <cstdio>
#include <stdlib.h>     /* atoi */
#include <algorithm>

#define _CPF_MAP_TOKEN_PREFIX "/$"
#define _CPF_TOKEN_PREFIX "/"
#define _CPF_TOKEN_SUFFIX "]"

std::uint8_t _cpf_colour_config = _CPF_ENABLE;
std::uint8_t _cpf_newline_config = _CPF_ENABLE;

const std::initializer_list<_cpf_type::str> attribute_escape_sequences = { 
	"`/", "`]", "`/$", "`|", "`;", "``", "`/@"
};
const std::initializer_list<char> std_format_specifiers = { 
	'c', 'd', 'e', 'E', 'f', 'g', 'i', 'o', 's', 'u', 'x' 
};
const std::initializer_list<char> extended_format_specifier_terminators = { 
	'd', 'f', 's', 'e', 'o', 'x' 
};
const std::initializer_list<char> intermediate_format_specifers = { 
	'+', '-', '.', '*', '#', 'l' 
};
const std::initializer_list<char> escape_characters = { 
	'\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\', '\"', '\0'
};


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

//	/$mystring1;mystring2|bld.r;b!]
std::map<_cpf_type::str, _cpf_type::str>
extract_map_token_values(const _cpf_type::str_pair str_frmt_pairs)
{
	auto get_num_semi_colons = [&](const _cpf_type::str &src)-> std::size_t
	{
		std::size_t p = 0, occur = 0, off =0;
		while ((p = _cpf_find(";", src, off)) != src.npos)
		{
			off = p + 1;
			++occur;
		}

		return occur;
	};

	std::map<_cpf_type::str, _cpf_type::str> out_meta;
	auto strings_str = str_frmt_pairs.first;
	auto frmts_str = str_frmt_pairs.second;

	auto num_strings = get_num_semi_colons(strings_str);
	// + 1 because number of colons is n - 1 where n is the number of tag strings
	num_strings += 1;

	auto num_fstrings = get_num_semi_colons(frmts_str);
	num_fstrings += 1;

	if (num_strings != num_fstrings && num_fstrings != 1)
	{
		throw _cpf_type::error("syntax error: key-value pair mismatch");
	}

	//mystring1;mystring2;mystring3
	//
	//bld.r;b!;yw
	std::size_t tstr_spos = 0, //tagged string start position
				tstr_epos = 0,  //tagged string end position
				fstr_spos = 0,  //tagged string's colour format string start position
				fstr_epos = 0;	//tagged string's colour format string end position

	do
	{
		tstr_epos = _cpf_find(";", strings_str, tstr_spos);
		auto tstr = strings_str.substr(tstr_spos, tstr_epos - tstr_spos);
		purge_str_esc_sequences(tstr);
		tstr_spos = tstr_epos + 1;

		fstr_epos = _cpf_find(";", frmts_str, fstr_spos);

		auto fstr = frmts_str.substr(fstr_spos, (fstr_epos - fstr_spos));

		if (num_fstrings > 1)
		{
			fstr_spos = fstr_epos + 1;
		}

		out_meta.insert(std::make_pair(tstr, fstr));

		--num_strings;
	} while (num_strings != 0);

	return out_meta;
}

_cpf_type::str _cpf_map_token_parse(
	const _cpf_type::str &src_format)
{
	_cpf_type::str	output_format_str;
	std::size_t token_suffix_pos = 0, 
				token_start_pos = 0;
	bool on_first_iteration = true;
	
	//"/$mystring1:mystring2|(bld;r)(b!)]"
	while ((token_start_pos = _cpf_find(_CPF_MAP_TOKEN_PREFIX, src_format, token_suffix_pos)) != src_format.npos)
	{
		/* if we have hit a tag map token effect inhibitor i.e /$]*/
		auto off_char_pos = token_start_pos + 2;
		auto off_char = src_format[off_char_pos];
		if (off_char == ']')
		{
			token_suffix_pos = off_char_pos;
			auto next_token_pos = _cpf_find(_CPF_MAP_TOKEN_PREFIX, src_format, token_suffix_pos);
			/*effectively eliminate token string from output string*/
			output_format_str.append(src_format.substr(token_suffix_pos + 1, next_token_pos - (token_suffix_pos + 1)));
			continue;
		}

		/*only on the first iteration and when pos is not the start point of the src_format string*/
		if (on_first_iteration && token_start_pos != 0)
		{
			on_first_iteration = false;
			output_format_str.append(src_format.substr(0, token_start_pos));
		}
		
		/*position of block space token delimiter*/
		token_suffix_pos = _cpf_find("]", src_format, token_start_pos);

		if (token_suffix_pos == src_format.npos)
		{
			throw _cpf_type::error("invalid map token: suffix not found");
		}

		/*... if there is one*/
		auto next_token_pos = _cpf_find(_CPF_MAP_TOKEN_PREFIX, src_format, token_suffix_pos);

		//+2 = length of "/$"
		auto raw_token = src_format.substr(token_start_pos + 2, (token_suffix_pos - token_start_pos) - 2);
		auto pipe_char_pos = _cpf_find("|", raw_token);
		if (pipe_char_pos == raw_token.npos)
		{
			throw _cpf_type::error("invalid map token: key-value pair delimiter '|' is missing");
		}
		_cpf_type::str_pair map_string(
			raw_token.substr(0, pipe_char_pos),
			raw_token.substr(pipe_char_pos + 1));

		auto parsed_meta_tokens = extract_map_token_values(map_string);

		auto tagged_section = src_format.substr(
									token_suffix_pos + 1, 
									(next_token_pos - (token_suffix_pos + 1))
									);

		for (auto &i : parsed_meta_tokens)
		{
			auto str_to_repl = i.first;
			_cpf_type::str repl_str;

			//reserve enough space for the number of character to be appended
			repl_str.reserve(5 + i.second.size() + str_to_repl.size());
			repl_str.append("/" + i.second + "]" + str_to_repl + "/!]");

			str_replace(tagged_section, str_to_repl, repl_str);
		}

		output_format_str.append(tagged_section);
	}

	if (!output_format_str.empty())
	{
		return output_format_str;
	}
	else
	{
		return src_format;
	}
}

void purge_meta_esc_sequences(_cpf_type::meta_format_type& meta)
{
	for (auto &e : meta)
	{
		purge_str_esc_sequences(e.second.second);
	}

	encountered_esc_seq_on_parse = false;
}

_cpf_type::meta_format_type _cpf_process_format_string(
	const _cpf_type::str &src_format)
{
	_cpf_type::meta_format_type meta;

	if (_cpf_colour_config == _CPF_DISABLE)
	{
		_cpf_type::str_vec default_attrib{ "no-colour" };
		meta.insert(std::make_pair(0, std::make_pair(default_attrib, src_format)));
		return meta;
	}

	_cpf_type::str _src_format = (_cpf_colour_config == _CPF_ENABLE) ? _cpf_map_token_parse(src_format) : src_format;

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_type::str::size_type start = 0;

		while ((start = _cpf_find(_CPF_TOKEN_PREFIX, _src_format, start)) != _cpf_type::str::npos)
		{
			++occurrences;
			start++;
		}
		return occurrences;
	}();
	
	std::size_t prefix_pos = 0, suffix_pos = 0;
	bool first_iter = true;
	while ((prefix_pos = _cpf_find(_CPF_TOKEN_PREFIX, _src_format, suffix_pos)) != _src_format.npos)
	{
		if(first_iter && prefix_pos != 0)
		{
			_cpf_type::str_vec default_attrib{ "!" };
			meta.insert(std::make_pair(0u, std::make_pair(default_attrib, _src_format.substr(0u, prefix_pos))));
			first_iter = false;
		}
		
		suffix_pos = _cpf_find(_CPF_TOKEN_SUFFIX, _src_format, prefix_pos);
		
		if(suffix_pos == _src_format.npos)
		{
			throw _cpf_type::error("invalid token encountered");
		}

		auto next_prefix_pos = _cpf_find(_CPF_TOKEN_PREFIX, _src_format, suffix_pos);
		auto attibs_str = _src_format.substr(prefix_pos + 1, (suffix_pos - 1) - prefix_pos);

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

		meta.insert(
			std::make_pair(
			prefix_pos, 
			std::make_pair(
				subseq_str_attribs, 
				_src_format.substr(suffix_pos + 1, (next_prefix_pos - (suffix_pos + 1))))));
	}

	if (meta.empty())
	{
		meta.insert(std::make_pair(0u, std::make_pair(_cpf_type::str_vec({ "!" }), src_format)));
	}

	if (encountered_esc_seq_on_parse)
	{
		purge_meta_esc_sequences(meta);
	}

	return meta;
}