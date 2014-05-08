#include "c_printf.h"
#include <stdlib.h>     /* atoi */
#include <algorithm>

extern "C" _cpf_types::colour _cpf_sys_attribs = S_T_A_UNDEF;
extern "C" _cpf_types::_string_type_ g_current_colour_repr = "S_T_A_UNDEF";

#ifdef _WIN32

auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);

#else /*	#ifdef _WIN32	*/

#endif /*	#ifdef _WIN32	*/

extern void _cpf_authenticate_format_string(const char* format)
{
	for	(; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw std::invalid_argument("bad format specifier");
	}
}

void _cpf_except_on_condition(bool condition, std::string _err_msg)
{
	if (condition == true)
	{
		throw std::invalid_argument(_err_msg);
	}
}

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
		delimiter_pos = src_format.find_first_of(']', pos);
		/*
		"i" -> (n + length("/¬"))  where n is "pos"
		*/
		std::size_t detail_spos = pos + bs_tag.size();

		auto err_msg = "syntax error: " + src_format.substr(detail_spos, (delimiter_pos - detail_spos));

		std::string repl_str_start_mark,
					repl_str_end_mark,
					/*	"/¬35<...>=..]"
					       ^^		*/
					repetition_counter,
					/*	"/¬...<foo>=...]"
					           ^^^		*/
					replacement_str, 
					/*	"/¬...<...>=r]"
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
		_cpf_except_on_condition((rblk_sze <= 0) , err_msg + "\nillegal replacement-string repetition counter.");
		_cpf_types::_string_type_ s_;

		for (auto i(0); i < rblk_sze; ++i)
		{
			s_.append(replacement_str);
		}

		output.append("/" + text_format_string + "]" + s_ + "/!]");
		auto offs = delimiter_pos + 1;
		auto t = src_format.find(bs_tag, offs);
		auto t_ = src_format.substr(offs, t);
		output.append(t_);
	}

	return output.size() != 0 ? output : src_format;;
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
					std::make_pair(	p_offset, //first
									std::make_pair(	c_repr, //second (first)
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
	const _cpf_types::_string_type_ &formatter )
{
	return _cpf_types::meta_format_type();
}

extern "C" std::size_t _cpf_get_num_arg_specifiers(
	const _cpf_types::_string_type_ & obj, 
	const _cpf_types::_string_type_ & target)
{
	std::size_t n = 0;
	_cpf_types::_string_type_::size_type pos = 0;
	while ((pos = obj.find(target, pos)) != _cpf_types::_string_type_::npos)
	{
		n++;
		pos += target.size();
	}
	return n;
}

extern "C" void _cpf_store_attribs(void)
{
	if (_cpf_sys_attribs == S_T_A_UNDEF)
	{
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console_stdout, &csbi);
		auto a = csbi.wAttributes;
		_cpf_sys_attribs = static_cast<_cpf_types::colour>(a % 16);
#else
		/*TODO:*/
#endif
	}
}

extern "C" void _cpf_load_attribs(void)
{
#ifdef _WIN32
	for (auto &handle : {console_stdout, console_stderr})
	{
		SetConsoleTextAttribute(handle, _cpf_sys_attribs);
	}
#else
	for (auto s : {stderr, stdout})
	{
		fprintf(s, "\x1B[0m");
	}
	
#endif
}

bool _cpf_is_fstream(_cpf_types::stream strm)
{
	bool is_fstream = true;
	for (auto s : { stdout, stderr })
	{
		if (strm == s)
		{
			is_fstream = false;
			break;
		}
	}
	return is_fstream;
}

extern "C" void _cpf_config_terminal(_cpf_types::stream strm,
	const _cpf_types::_string_type_ c_repr)
{
	if (_cpf_is_fstream(strm))
	{
		return;
	}

	if (g_current_colour_repr.compare(c_repr) != 0)
	{
		g_current_colour_repr = c_repr;
	}

#ifdef _WIN32
	HANDLE hnd = nullptr;//TODO: test this for mem leaks
	if (strm == stdout)
	{
		hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else if (strm == stderr)
	{
		hnd = GetStdHandle(STD_ERROR_HANDLE);
	}
	assert(hnd != nullptr);
	SetConsoleTextAttribute(hnd, _cpf_colour_token_vals.find(c_repr)->second);
#else
	fprintf(strm, 
			"%s", 
			_cpf_colour_token_vals.find(g_current_colour_repr)->second.c_str());
	
#endif
}

_cpf_types::_string_type_ _cpf_print_pre_arg_str(	_cpf_types::stream strm,
													_cpf_types::_string_type_& printed_string_,
													std::size_t& ssp_)
{
	ssp_ = printed_string_.find_first_of("%", ssp_);
	if (ssp_ != 0)
	{
		fprintf(strm, "%s", printed_string_.substr(0, ssp_).c_str());
	}

	auto offset = 2;
	auto fstr = printed_string_.substr(ssp_, offset);
	ssp_ += offset;
	return fstr;
}

void _cpf_print_post_arg_str(	_cpf_types::stream strm,
								_cpf_types::_string_type_& printed_string_,
								std::size_t& ssp_,
								bool &more_args_on_iter,
								_cpf_types::meta_format_type::const_iterator &msd_iter)
{
	printed_string_ = printed_string_.substr(ssp_);
	ssp_ = 0;

	more_args_on_iter = _cpf_get_num_arg_specifiers(printed_string_, "%") > 0;
	if (!more_args_on_iter)
	{
		if (!printed_string_.empty())
		{
			fprintf(strm, "%s", printed_string_.c_str());
			printed_string_.clear();
		}
		std::advance(msd_iter, 1);
	}
}

void _cpf_print_non_arg_str(_cpf_types::stream strm,
							_cpf_types::_string_type_& printed_string_,
							std::size_t& ssp_,
							_cpf_types::meta_format_type::const_iterator &msd_iter)
{
	ssp_ = 0;
	fprintf(strm, "%s", printed_string_.c_str());
	std::advance(msd_iter, 1);

	while (_cpf_get_num_arg_specifiers(msd_iter->second.second, "%") == 0)
	{
		_cpf_config_terminal(strm, msd_iter->second.first);
		fprintf(strm, "%s", msd_iter->second.second.c_str());
		std::advance(msd_iter, 1);
	}
}

void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
	const _cpf_types::_string_type_ printed_string="",
	const std::size_t search_start_pos=0)
{
	while (msd_iter != end_point_comparator)
    {
		_cpf_config_terminal(strm, msd_iter->second.first);
		fprintf(strm, "%s", msd_iter->second.second.c_str());
		std::advance(msd_iter, 1);
    }

	_cpf_load_attribs();
}
