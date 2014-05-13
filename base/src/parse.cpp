#include "parse.h"
#include "colour_repr.h"
#include <cstdio>
#include <stdlib.h>     /* atoi */
#include <algorithm>
#include <sstream>

/*

*/
_cpf_types::_string_type_ _cpf_block_space_token_parse(
	const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::_string_type_	output_format_str, 
								token_prefix = "/¬", 
								token_suffix = "¬]";
	std::size_t token_suffix_pos = 0;
	std::size_t token_start_pos = 0;
	bool on_first_iteration = true;
	while ((token_start_pos = src_format.find(token_prefix, token_suffix_pos)) != src_format.npos)
	{
		if (token_start_pos != 0 && on_first_iteration)
		{
			on_first_iteration = false;
			output_format_str.append(src_format.substr(0, token_start_pos));
		}

		/*position of block space token delimiter*/
		token_suffix_pos = src_format.find(token_suffix, token_start_pos);
		/*
		"i" -> (n + length("/¬"))  where n is "pos"
		*/
		std::size_t detail_spos = token_start_pos + token_prefix.size();

		auto err_msg = "syntax error: " + src_format.substr(detail_spos, (token_suffix_pos - detail_spos));

		std::string repl_str_start_mark,
			repl_str_end_mark,
			/*	"/¬35<...>=..¬]"
			^^		*/
			repetition_counter,
			/*	"/¬...<foo>=...¬]"
			^^^		*/
			replacement_str,
			/*	"/¬...<...>=r¬]"
			^
			colour and/or format string */
			text_format_string;

		repl_str_start_mark = "<";
		repl_str_end_mark = ">";

		// "<"
		auto bssm_pos = src_format.find_first_of(repl_str_start_mark, token_start_pos);
		_cpf_except_on_condition(bssm_pos == std::string::npos, err_msg);

		auto bsem_pos = src_format.find_first_of(repl_str_end_mark, token_start_pos);
		_cpf_except_on_condition(bssm_pos == std::string::npos, err_msg);
		_cpf_except_on_condition(((bsem_pos - bssm_pos) < 1), err_msg + "\nkey string undefined.");

		auto c_token_start_pos = src_format.find_first_of(">=", token_start_pos);
		_cpf_except_on_condition(c_token_start_pos == std::string::npos, err_msg);
		c_token_start_pos += 2; //length of ">="

		auto sub_parse = [&](const std::size_t start, const std::size_t end, std::string& out)->void
		{
			auto index = start;
			while (index < end)
			{
				out.append({ src_format[index++] });
			}
		};

		sub_parse(detail_spos, bssm_pos, repetition_counter);

		sub_parse(bssm_pos + 1, bsem_pos, replacement_str);

		sub_parse(c_token_start_pos, token_suffix_pos, text_format_string);

		auto rblk_sze = atol(repetition_counter.c_str());
		_cpf_except_on_condition((rblk_sze <= 0), err_msg + "\nillegal replacement-string repetition counter.");
		_cpf_types::_string_type_ s_, col_str;
		col_str = "/" + text_format_string + "]";
		for (auto i(0); i < rblk_sze; ++i)
		{
			s_.append(col_str + replacement_str);
		}

		/* /!] is used used reset any formatting options set by the block space format string*/
		output_format_str.append(col_str + s_ + "/!]");
		auto offs = token_suffix_pos + 1;

		auto t = src_format.find(token_prefix, offs);
		auto t_ = src_format.substr(offs, t);
		if (t != std::string::npos)
		{
			output_format_str.append(t_);
		}
	}

	return output_format_str.size() != 0 ? output_format_str : src_format;;
}

//	{mystring1:mystring2}|(bld;r)(b!)
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
		[&](char e){ return e == ':'; });
	// + 1 because number of colons is n - 1 where n is the number of tag strings
	num_strings += 1;

	auto num_fstrings = std::count(std::begin(frmts_str),
		std::end(frmts_str), '(');

	_cpf_except_on_condition((num_strings != num_fstrings && num_fstrings != 1),
		"syntax error: tag token");

	//mystring1:mystring2:mystring3
	//
	//(bld;r)(b!)(yw)
	std::size_t tstr_spos = 0, //tagged string start position
				tstr_epos = 0,  //tagged string end position
				fstr_spos = 0,  //tagged string's colour format string start position
				fstr_epos = 0;	//tagged string's colour format string end position

	do
	{
		tstr_epos = strings_str.find(":", tstr_spos);
		auto tstr = strings_str.substr(tstr_spos, tstr_epos - tstr_spos);
		tstr_spos = tstr_epos + 1;

		fstr_spos = frmts_str.find("(", fstr_spos);
		fstr_epos = frmts_str.find(")", fstr_spos);

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

#ifdef __gnu_linux__

_cpf_types::_string_type_ _cpf_full_colour_spectrum_token_parse(const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::_string_type_ output_format_str;

	const _cpf_types::_string_type_ prefix =  "/&",
									suffix = "]";

	std::size_t token_suffix_pos = 0, 
				token_start_pos = 0;
	bool on_first_iteration = true;
	
	//	"/&bld;24f;128b]"
	while ((token_start_pos = src_format.find(prefix, token_suffix_pos)) != src_format.npos)
	{
		if(on_first_iteration && token_start_pos != 0)
		{
			output_format_str.append(src_format.substr(0, token_start_pos));
			on_first_iteration = false;
		}

		token_suffix_pos = src_format.find(suffix, token_start_pos);
		_cpf_except_on_condition(	token_suffix_pos >= src_format.size(), 
									"invalid full colour spectrum token");

		_cpf_types::_string_type_ value_str;
		auto i = token_start_pos + 2; //+2 -> len("/&")
		while(i < token_suffix_pos)
		{
			value_str.append({src_format[i++]});
		}

		_cpf_types::string_vector f_strings;
		_cpf_types::_string_type_ segment;
		std::stringstream ssrt;
		ssrt << value_str;
		while (std::getline(ssrt, segment, ';'))
		{
			f_strings.push_back(segment);
		}

		/*append the text formatting strings to the output string*/
		for(auto &i : f_strings)
		{
			_cpf_types::_string_type_ colour_str;
			
			char scnd_lst_char = i[i.size() - 2];
			bool is_ful_spec_col_token = isdigit(scnd_lst_char);
			if(is_ful_spec_col_token)//if its a full colour spectrum token i.e 76f
			{
				char lst_char = i[i.size() - 1];
				auto colour_num = i.substr(0, i.size() - 2);
				if(lst_char == 'f')//foreground
				{
					colour_str = ("\x1B[38;5;" + colour_num + "m");
				}
				else if(lst_char == 'b') //background
				{
					colour_str = ("\x1B[48;5;" + colour_num + "m"); 
				}
				else
				{
					_cpf_except_on_condition(true, "invalid full-colour-spectrum token.");
				}
			}
			
			output_format_str.append( 
				is_ful_spec_col_token ? colour_str : _cpf_colour_token_vals.find(i)->second);
		}

		auto next_token_pos = src_format.find(prefix, token_suffix_pos);
		output_format_str.append(src_format.substr(	token_suffix_pos + 1, 
													next_token_pos - (token_suffix_pos + 1)));
	}

	if(!output_format_str.empty())
	{
		return output_format_str;
	}
	else
	{
		return src_format;
	}
}

#endif
/*
	USE std::find_if IN ALL PARSING FUNCTIONS in order to be able to use a custom
	search algorithm meaning it will be easier to escape the forward slashes using a lambda for example
*/
_cpf_types::meta_format_type _cpf_colour_token_parse(
	const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::_string_type_ _src_format;
#ifdef __gnu_linux__
	_src_format = _cpf_full_colour_spectrum_token_parse(src_format);
#else
	//windows
	_src_format = src_format;
#endif
	_cpf_types::meta_format_type meta;

	_cpf_types::_string_type_ 	_c_prefix = "/", 
								_c_suffix = "]";

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_types::_string_type_::size_type start = 0;

		while ((start = _src_format.find(_c_prefix, start)) != _cpf_types::_string_type_::npos)
		{
			++occurrences;
			start += _c_prefix.length();
		}
		return occurrences;
	}();

	auto first_c_frmt_pos = _src_format.find(_c_prefix);
	if (first_c_frmt_pos != 0)
	{
		meta.insert(
			std::make_pair(	0, 
							std::make_pair("!", 
											_src_format.substr(0, first_c_frmt_pos)))
			);
	}

	/*not that the followng only searches for the "standardised" colour tokens
		i.e /y!] and not the full colour spectrum tokens i.e /#34f]*/
	std::size_t counter = 0;
	for (auto &c_repr : _cpf_colour_tokens)
	{
		if (counter > NUM_C_TAGS)
		{
			break;
		}
		auto c_frmt = _c_prefix + c_repr + _c_suffix;
		auto token_start_pos = _src_format.find(c_frmt);

		while (token_start_pos != _src_format.npos)
		{
			auto p_ = token_start_pos;
			auto p_offset = p_ + c_frmt.length();
			token_start_pos = _src_format.find(_c_prefix, p_offset);
			auto cf = _src_format.substr(p_, c_frmt.length());

			if (cf == c_frmt)
			{
				meta.insert(
					std::make_pair(p_offset, //first
						std::make_pair(c_repr, //second (first)
						_src_format.substr(p_offset, //(second) 
						(token_start_pos - p_offset)))));

				token_start_pos = _src_format.find(c_frmt, p_offset);
				++counter;
			}
		}
	}

	return meta;
}

extern void _cpf_authenticate_format_string(const char* format)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw std::invalid_argument("bad format specifier");
	}
}