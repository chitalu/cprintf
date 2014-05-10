#include "c_printf.h"
#include "colour_repr.h"

extern "C" _cpf_types::colour _cpf_sys_attribs = S_T_A_UNDEF;
extern "C" _cpf_types::_string_type_ g_current_colour_repr = "S_T_A_UNDEF";

#ifdef _WIN32
auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);
#else /*	#ifdef _WIN32	*/

#endif /*	#ifdef _WIN32	*/

void _cpf_except_on_condition(bool condition, std::string _err_msg)
{
	if (condition == true)
	{
		throw std::invalid_argument(_err_msg);
	}
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
