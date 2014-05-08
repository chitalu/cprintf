#include <stdexcept>

#include "common.h"

extern "C" _cpf_types::_string_type_ g_current_colour_repr;
extern "C" const _cpf_types::string_vector _cpf_colour_tokens;

/*
	descr
	*\

	args
	*\@

*/
extern void _cpf_authenticate_format_string(const char* format);

/*
	descr
	*\

	args
	*\@

*/
template<class T, typename... Ts> void
_cpf_authenticate_format_string(const char* format, const T& farg, const Ts&&... args)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		char f = *format;
		switch (f)
		{
		case 'f':
			assert(std::is_floating_point<T>::value);
			break;
		case 'd':
			assert(std::is_integral<T>::value);
			break;
		case 'c':
			assert(std::is_integral<T>::value);
			break;
		case 's':
			assert(std::is_pointer<T>::value);
			break;
		default:
			printf("warning: unsupported formatter: %c\n", f);
			break;
		}
		return check_printf(++format, args...);
	}

	throw std::invalid_argument("Too few format specifiers.");
}

/*
	descr
	*\

	args
	*\@

*/
extern _cpf_types::_string_type_ _cpf_do_block_space_parse(
	const _cpf_types::_string_type_ &src_format);

/*
	descr
	*\

	args
	*\@

*/
extern _cpf_types::meta_format_type _cpf_do_colour_token_parse(
	const _cpf_types::_string_type_ &formatter );

/*
	descr
	*\

	args
	*\@

*/
extern _cpf_types::meta_format_type _cpf_do_cursor_position_parse(
	const _cpf_types::_string_type_ &formatter );

/*

*/
extern void _cpf_except_on_condition(bool condition, std::string _err_msg);

/*
	descr
	*\

	args
	*\@

*/
extern "C" void _cpf_store_attribs(void);

/*
	descr
	*\

	args
	*\@

*/
extern "C" void _cpf_load_attribs(void);

/*
descr
*\

args
	*\@strm - output stream

*/
extern "C" bool _cpf_is_fstream(_cpf_types::stream strm);

/*
	descr
	*\configure system terminal settings
	
	args
	*\	@strm 	- standard output stream
	*\	@c_repr - colour token string used to locate corresponding value
*/
extern "C" void _cpf_config_terminal(	_cpf_types::stream strm, 
										const _cpf_types::_string_type_ c_repr);

/*
	descr
	*\

	args
	*\@

*/
extern "C" std::size_t _cpf_get_num_arg_specifiers(	const _cpf_types::_string_type_ & obj, 
													const _cpf_types::_string_type_ & str);

/*
	descr
*\

args
*\@

*/
extern _cpf_types::_string_type_ _cpf_print_pre_arg_str(_cpf_types::stream strm,
														_cpf_types::_string_type_& printed_string_,
														std::size_t& ssp_);

/*
descr
*\

args
*\@

*/
extern void _cpf_print_post_arg_str(_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									bool &more_args_on_iter,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*
descr
*\

args
*\@

*/
extern void _cpf_print_non_arg_str(	_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*
	descr
	*\

	args
	*\@

*/
extern void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
	const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos);

/*
	descr
	*\

	args
	*\@

*/
template<typename T0, typename ...TN>
void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
    const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos,
	T0&& arg0,
    TN&&... args)
{
	_cpf_config_terminal(strm, msd_iter->second.first);

	_cpf_types::_string_type_ printed_string_ = printed_string;

	/*printed string argument ('%') count*/
	const auto pstr_argc = _cpf_get_num_arg_specifiers(printed_string_, "%");
	
	/*more printf args to print using "printed_string_" as format	*/
	bool more_args_on_iter = false;

	/*boolean used to determine whether variadic arg0 has been passed to printf as an argument yet	*/
	bool printed_arg0 = false;

	/*string parsing start position...*/
    auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		auto fstr = _cpf_print_pre_arg_str(strm, printed_string_, ssp_);
		fprintf(strm, fstr.c_str(), arg0);
		_cpf_print_post_arg_str(strm, printed_string_, ssp_, more_args_on_iter, msd_iter);
		printed_arg0 = true;
	}
	else
	{
		_cpf_print_non_arg_str(strm, printed_string_, ssp_, msd_iter);
	}

	bool iter_reached_end = (msd_iter == end_point_comparator);
	auto i_raw_str = msd_iter->second.second;
	if (printed_arg0)
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
            more_args_on_iter, std::forward<TN>(args)...);
    }
	else
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
			ssp_, arg0, std::forward<TN>(args)...);
    }
}

/*
	descr
	*\

	args
	*\@

*/
template<typename... TN>
void c_printf(	_cpf_types::stream strm, const char* format, TN... args)
{
	_cpf_types::error err = 0;

	if (strm == nullptr)
	{ 
		throw std::invalid_argument("output stream undefined");
	}
	else if(format == nullptr)
	{
		throw std::invalid_argument("format undefined");
	}

#if defined(NDEBUG) || defined(_DEBUG)
	assert(
		(_cpf_get_num_arg_specifiers(format, "%") > 0 && sizeof...(args) > 0) ||
		(_cpf_get_num_arg_specifiers(format, "%") == 0 && sizeof...(args) == 0)
		);
	//check_printf(format, normalize_arg(args)...);
#endif

	auto block_space_parsed_str = _cpf_do_block_space_parse(format);
    auto meta_string_data = _cpf_do_colour_token_parse(block_space_parsed_str);
    
    auto msd_iter_begin = meta_string_data.cbegin();
    auto m_iter_end_point_comparator = meta_string_data.cend();

   /*for(auto &i : meta_string_data)
   {
   		auto c_meta = _cpf_do_cursor_position_parse(i.second.second);
   		auto c_iter_begin = meta_string_data.cbegin();
    	auto c_iter_end_point_comparator = meta_string_data.cend();
   		
   		_cpf_call_(	strm,
					c_iter_end_point_comparator,
					c_iter_begin,
					c_iter_begin->second.second,
					0,
					normalize_arg(args)...);
   }*/

	_cpf_call_(	strm,
				m_iter_end_point_comparator,
				msd_iter_begin,
				msd_iter_begin->second.second,
				0,
				std::forward<TN>(normalize_arg(args))...);
}

