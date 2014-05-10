#include "parse.h"
#include "colour_repr.h"
#include <cstdio>
#include <stdlib.h>     /* atoi */
#include <algorithm>
#include <sstream>

/*

*/
_cpf_types::_string_type_ _cpf_do_block_space_parse(
	const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::_string_type_ output;
	_cpf_types::_string_type_ bs_tag = "/¬";
	std::size_t delimiter_pos = 0;
	std::size_t pos = 0;
	bool on_first_iteration = true;
	while ((pos = src_format.find(bs_tag, delimiter_pos)) != src_format.npos)
	{
		if (pos != 0 && on_first_iteration)
		{
			on_first_iteration = false;
			output.append(src_format.substr(0, pos));
		}

		/*position of block space token delimiter*/
		delimiter_pos = src_format.find("¬]", pos);
		/*
		"i" -> (n + length("/¬"))  where n is "pos"
		*/
		std::size_t detail_spos = pos + bs_tag.size();

		auto err_msg = "syntax error: " + src_format.substr(detail_spos, (delimiter_pos - detail_spos));

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
		auto bssm_pos = src_format.find_first_of(repl_str_start_mark, pos);
		_cpf_except_on_condition(bssm_pos == std::string::npos, err_msg);

		auto bsem_pos = src_format.find_first_of(repl_str_end_mark, pos);
		_cpf_except_on_condition(bssm_pos == std::string::npos, err_msg);
		_cpf_except_on_condition(((bsem_pos - bssm_pos) < 1), err_msg + "\nkey string undefined.");

		auto c_token_start_pos = src_format.find_first_of(">=", pos);
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

		sub_parse(c_token_start_pos, delimiter_pos, text_format_string);

		auto rblk_sze = atol(repetition_counter.c_str());
		_cpf_except_on_condition((rblk_sze <= 0), err_msg + "\nillegal replacement-string repetition counter.");
		_cpf_types::_string_type_ s_, col_str;
		col_str = "/" + text_format_string + "]";
		for (auto i(0); i < rblk_sze; ++i)
		{
			s_.append(col_str + replacement_str);
		}

		/* /!] is used used reset any formatting options set by the block space format string*/
		output.append(col_str + s_ + "/!]");
		auto offs = delimiter_pos + 1;

		auto t = src_format.find(bs_tag, offs);
		auto t_ = src_format.substr(offs, t);
		if (t != std::string::npos)
		{
			output.append(t_);
		}
	}

	return output.size() != 0 ? output : src_format;;
}

//	{mystring1:mystring2}|(bld;r)(b!)
std::map<std::string, _cpf_types::string_vector>
parse_tag_space_token(const _cpf_types::str_pair str_frmt_pairs)
{

	auto parse_frmt = [&](const _cpf_types::_string_type_ target_str,
		const _cpf_types::_string_type_& src_format,
		std::map<std::string, _cpf_types::string_vector>& str_frmt_map)
	{
		_cpf_types::string_vector svec;
		_cpf_types::_string_type_ current_str_tag;

		for (auto i(0u); i < src_format.size(); ++i)
		{
			if (src_format[i] == ';')
			{
				svec.push_back(current_str_tag);
				current_str_tag.clear();
				continue;
			}
			current_str_tag.append({ src_format[i] });

			if (i == src_format.size() - 1)
			{
				svec.push_back(current_str_tag);
				current_str_tag.clear();
			}
		}

		str_frmt_map.insert(std::make_pair(target_str, svec));
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

	auto colon_pos = strings_str.find(":");

	//mystring1:mystring2:mystring3
	//
	//(bld;r)(b!)(yw)
	std::size_t tstr_spos = 0, tstr_epos = 0, fstr_spos = 0, fstr_epos = 0;
	bool made_first_pass = false;

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

_cpf_types::_string_type_ _cpf_do_sub_str_tag_token_parse(
	_cpf_types::_string_type_ &src_format)
{
	auto str_replace = [&](std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	};

	_cpf_types::_string_type_ output;
	_cpf_types::_string_type_ tag = "/$";
	std::size_t delimiter_pos = 0;
	std::size_t pos = 0;
	//	"`"
	std::size_t crnt_end_quote_pos = 0, crnt_begin_quote_pos = 0;
	bool on_first_iteration = true;
	std::map<std::string, _cpf_types::string_vector> str_frmt_map;
	//"/${mystring1:mystring2}|(bld;r)(b!)$]"
	while ((pos = src_format.find(tag, delimiter_pos)) != src_format.npos)
	{
		if (on_first_iteration && pos != 0)
		{
			on_first_iteration = false;
			output.append(src_format.substr(0, pos));
		}
		std::size_t pipe_char_pos = src_format.find("|", pos);
		/*position of block space token delimiter*/
		delimiter_pos = src_format.find("]", pos);

		auto next_tag_token_pos = src_format.find(tag, delimiter_pos);
		/*effectively eliminate token string from output string*/
		output.append(src_format.substr(delimiter_pos + 2, next_tag_token_pos - delimiter_pos));

		//+2 = length of "/$"
		auto raw_token = src_format.substr(pos + 2, (delimiter_pos - pos) - 2);
		_cpf_types::str_pair str_frmts;
		_cpf_types::_string_type_ segment;
		auto ssrt = std::stringstream(raw_token);
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

		auto parsed_meta_tokens = parse_tag_space_token(str_frmts);
		str_frmt_map.insert(std::begin(parsed_meta_tokens), std::end(parsed_meta_tokens));
	}

	if (!str_frmt_map.empty())
	{
		for (auto &i : str_frmt_map)
		{
			auto str_to_repl = i.first;
			_cpf_types::_string_type_ repl_str;

			for (auto &j : i.second)
			{
				repl_str.append("/" + j + "]");
			}

			repl_str.append(str_to_repl + "/!]");

			str_replace(output, str_to_repl, repl_str);
		}
		return output;
	}
	else
	{
		return src_format;
	}
}

_cpf_types::meta_format_type _cpf_do_colour_token_parse(
	const _cpf_types::_string_type_ &formatter)
{
	_cpf_types::meta_format_type meta;

	_cpf_types::_string_type_ _c_prefix = "/", _c_suffix = "]";

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_types::_string_type_::size_type start = 0;

		while ((start = formatter.find(_c_prefix, start)) != _cpf_types::_string_type_::npos)
		{
			++occurrences;
			start += _c_prefix.length();
		}
		return occurrences;
	}();

	auto first_c_frmt_pos = formatter.find(_c_prefix);
	if (first_c_frmt_pos != 0)
	{
		meta.insert(
			std::make_pair(0, std::make_pair("!", formatter.substr(0, first_c_frmt_pos)))
			);
	}

	std::size_t counter = 0;
	for (auto &c_repr : _cpf_colour_tokens)
	{
		if (counter > NUM_C_TAGS)
		{
			break;
		}
		auto c_frmt = _c_prefix + c_repr + _c_suffix;
		auto pos = formatter.find(c_frmt);

		while (pos != formatter.npos)
		{
			auto p_ = pos;
			auto p_offset = p_ + c_frmt.length();
			pos = formatter.find(_c_prefix, p_offset);
			auto cf = formatter.substr(p_, c_frmt.length());

			if (cf == c_frmt)
			{
				meta.insert(
					std::make_pair(p_offset, //first
					std::make_pair(c_repr, //second (first)
					formatter.substr(p_offset, //(second) 
					(pos - p_offset))
					)
					)
					);
				pos = formatter.find(c_frmt, p_offset);
				++counter;
			}
		}
	}

	return meta;
}

_cpf_types::meta_format_type _cpf_do_cursor_position_parse(
	const _cpf_types::_string_type_ &formatter)
{
	return _cpf_types::meta_format_type();
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