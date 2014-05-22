#include "_cpf_parse.h"
#include "_cpf_sys_colour_config.h"
#include "_cpf_find.h"
#include <cstdio>
#include <stdlib.h>     /* atoi */
#include <algorithm>
#include <sstream>

#define _CPF_MAP_TOKEN_PREFIX "/$"
#define _CPF_TOKEN_PREFIX "/"
#define _CPF_TOKEN_SUFFIX "]"

std::uint8_t _cpf_colour_config = _CPF_ENABLE;
std::uint8_t _cpf_newline_config = _CPF_ENABLE;

//	/$mystring1;mystring2|bld.r;b!]
std::map<std::string, _cpf_types::string_vector>
parse_tag_map_token_values(const _cpf_types::str_pair str_frmt_pairs)
{
	auto parse_frmt = [&](const _cpf_types::_string_type_ target_str,
		const _cpf_types::_string_type_& src_format,
		std::map<std::string, _cpf_types::string_vector>& str_frmt_map)->void
	{
		_cpf_types::string_vector tagged_strings;
		_cpf_types::_string_type_ current_str_tag;

		for (auto i(0u); i < src_format.size(); ++i)
		{
			if (src_format[i] == ';')
			{
				tagged_strings.push_back(current_str_tag);
				current_str_tag.clear();
				continue;
			}
			current_str_tag.append({ src_format[i] });

			if (i == src_format.size() - 1)
			{
				tagged_strings.push_back(current_str_tag);
				current_str_tag.clear();
			}
		}

		str_frmt_map.insert(std::make_pair(target_str, tagged_strings));
	};

	std::map<std::string, _cpf_types::string_vector> out_meta;
	auto strings_str = str_frmt_pairs.first;
	auto frmts_str = str_frmt_pairs.second;

	auto num_strings = std::count_if(std::begin(strings_str),
		std::end(strings_str),
		[&](char e){ return e == ';'; });
	// + 1 because number of colons is n - 1 where n is the number of tag strings
	num_strings += 1;

	auto num_fstrings = std::count(std::begin(frmts_str),
		std::end(frmts_str), ';');
	num_fstrings += 1;

	_cpf_except_on_condition((num_strings != num_fstrings && num_fstrings != 1),
		"syntax error: map token");

	//mystring1;mystring2;mystring3
	//
	//bld.r;b!;yw
	std::size_t tstr_spos = 0, //tagged string start position
				tstr_epos = 0,  //tagged string end position
				fstr_spos = 0,  //tagged string's colour format string start position
				fstr_epos = 0;	//tagged string's colour format string end position

	do
	{
		tstr_epos = strings_str.find(";", tstr_spos);
		auto tstr = strings_str.substr(tstr_spos, tstr_epos - tstr_spos);
		tstr_spos = tstr_epos + 1;

		fstr_spos = frmts_str.find("(", fstr_spos);
		fstr_epos = frmts_str.find(";", fstr_spos);

		auto fstr = frmts_str.substr(fstr_spos + 1, (fstr_epos - fstr_spos) - 1);

		if (num_fstrings > 1)
		{
			fstr_spos = fstr_epos + 1;
		}

		parse_frmt(tstr, fstr, out_meta);

		--num_strings;
	} while (num_strings != 0);

	return out_meta;
}

_cpf_types::_string_type_ _cpf_tag_map_token_parse(
	const _cpf_types::_string_type_ &src_format)
{
	/*string replacement lambda*/
	auto str_replace = [&](	std::string& subject, 
							const std::string& search, 
							const std::string& replace)->void
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	};

	_cpf_types::_string_type_	output_format_str, 
								token_prefix = "/$";
	std::size_t token_suffix_pos = 0, 
				token_start_pos = 0;
	bool on_first_iteration = true;
	
	//"/$mystring1:mystring2|(bld;r)(b!)]"
	while ((token_start_pos = src_format.find(token_prefix, token_suffix_pos)) != src_format.npos)
	{
		/* if we have hit a tag map token effect inhibitor i.e /$]*/
		auto off_char_pos = token_start_pos + 2;
		auto off_char = src_format[off_char_pos];
		if (off_char == ']')
		{
			token_suffix_pos = off_char_pos;
			auto next_token_pos = src_format.find(token_prefix, token_suffix_pos);
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
		std::size_t pipe_char_pos = src_format.find("|", token_start_pos);
		/*position of block space token delimiter*/
		token_suffix_pos = src_format.find("]", token_start_pos);

		/*... if there is one*/
		auto next_token_pos = src_format.find(token_prefix, token_suffix_pos);

		//+2 = length of "/$"
		auto raw_token = src_format.substr(token_start_pos + 2, (token_suffix_pos - token_start_pos) - 2);
		_cpf_types::str_pair str_frmts;
		_cpf_types::_string_type_ segment;
		//auto ssrt = std::stringstream(raw_token);
		std::stringstream ssrt;
		ssrt << raw_token;
		//insure non syntax pipe chars are escaped else throw 
		while (std::getline(ssrt, segment, '|'))
		{
			if (str_frmts.first.empty())
			{
				str_frmts.first = segment;
			}
			else
			{
				str_frmts.second = segment;
			}
		}

		auto parsed_meta_tokens = parse_tag_map_token_values(str_frmts);
		auto tagged_section = src_format.substr(
									token_suffix_pos + 1, 
									(next_token_pos - (token_suffix_pos + 1))
									);

		for (auto &i : parsed_meta_tokens)
		{
			auto str_to_repl = i.first;
			_cpf_types::_string_type_ repl_str;

			for (auto &j : i.second)
			{
				repl_str.append("/" + j + "]");
			}

			repl_str.append(str_to_repl + "/!]");

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

_cpf_types::meta_format_type _cpf_process_format_string(
	const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::meta_format_type meta;

	if (_cpf_colour_config == _CPF_DISABLE)
	{
		_cpf_types::string_vector default_attrib{ "no-colour" };
		meta.insert(std::make_pair(0, std::make_pair(default_attrib, src_format)));
		return meta;
	}

	_cpf_types::_string_type_ _src_format = (_cpf_colour_config == _CPF_ENABLE) ? _cpf_tag_map_token_parse(src_format) : src_format;

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_types::_string_type_::size_type start = 0;

		while ((start = _src_format.find(_CPF_TOKEN_PREFIX, start)) != _cpf_types::_string_type_::npos)
		{
			++occurrences;
			start++;
		}
		return occurrences;
	}();
	
	auto prefix_pos = 0, suffix_pos = 0;
	bool first_iter = true;
	while ((prefix_pos = src_format.find(_CPF_TOKEN_PREFIX, suffix_pos)) != src_format.npos)
	{
		if(first_iter && prefix_pos != 0)
		{
			_cpf_types::string_vector default_attrib{ "!" };
			meta.insert(std::make_pair(0u, std::make_pair(default_attrib, _src_format.substr(0u, prefix_pos))));
			first_iter = false;
		}
		
		suffix_pos = _src_format.find(_CPF_TOKEN_SUFFIX, prefix_pos);
		
		if(suffix_pos == _src_format.npos)
		{
			std::ostringstream convert;
			convert << prefix_pos;
			throw _cpf_types::error(std::string("invalid attribute at position: ").append(convert.str()).c_str());
		}

		auto next_prefix_pos = _src_format.find(_CPF_TOKEN_PREFIX, suffix_pos);
		auto attibs_str = _src_format.substr(prefix_pos + 1, (suffix_pos - 1) - prefix_pos);

		_cpf_types::string_vector subseq_str_attribs;
		_cpf_types::_string_type_ current_attrib;

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

	return meta;
}