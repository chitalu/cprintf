#include "c_printf.h"
#include "_cpf_sys_colour_config.h"

/*text attributes before a call was made to c_printf*/
_cpf_types::colour _cpf_default_sys_attribs = SYSTXTATTIB_UNDEF;

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

_cpf_types::_string_type_ _cpf_print_pre_arg_str(	_cpf_types::stream strm,
													_cpf_types::_string_type_& printed_string_,
													std::size_t& ssp_,
													const _cpf_types::attributes attr)
{
	if (_cpf_colour_config == _CPF_ENABLE)
	{
		_cpf_config_terminal(strm, attr);
	}

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
	if (_cpf_colour_config == _CPF_ENABLE)
	{
		_cpf_config_terminal(strm, msd_iter->second.first);
	}

	ssp_ = 0;
	fprintf(strm, "%s", printed_string_.c_str());
	std::advance(msd_iter, 1);

	while (_cpf_get_num_arg_specifiers(msd_iter->second.second, "%") == 0)
	{
		if (_cpf_colour_config == _CPF_ENABLE)
		{
			_cpf_config_terminal(strm, msd_iter->second.first);
		}
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
		auto nl =	std::distance(msd_iter, end_point_comparator) > 1 && 
					(_cpf_newline_config == _CPF_ENABLE);
		if (_cpf_colour_config == _CPF_ENABLE)
		{
			_cpf_config_terminal(strm, msd_iter->second.first);
		}
		fprintf(strm, nl ? "%s" : "%s\n", msd_iter->second.second.c_str());
		std::advance(msd_iter, 1);
    }

	/*restore defaults*/
	if (_cpf_colour_config == _CPF_ENABLE)
	{
		_cpf_config_terminal(strm, msd_iter->second.first);
	}
}
