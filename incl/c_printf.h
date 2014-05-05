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
extern "C" void _cpf_authenticate_format_string(const char* format);

/*
	descr
	*\

	args
	*\@

*/
template<class T, typename... Ts> void
_cpf_authenticate_format_string(const char* format, const T& farg, const Ts&... args)
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
extern _cpf_types::error _cpf_call_(	
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
_cpf_types::error _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
    const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos,
	const T0 arg0,
    const TN... args)
{
	_cpf_config_terminal(strm, msd_iter->second.first);

	_cpf_types::error err = 0;
	_cpf_types::_string_type_ printed_string_ = printed_string;

	/*
		*\printed string argument ('%') count
	*/
	const auto pstr_argc = _cpf_get_num_arg_specifiers(printed_string_, "%");
	
	/*
		*\more printf args to print using "printed_string_" as format
	*/
	bool more_args_on_iter = false;

	/*
		*\boolean used to determine whether variadic arg0 has been 
		*\passed to printf as an argument yet
	*/
	bool printed_arg0 = false;

	/*
		*\string parsing start position...
	*/
    auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		ssp_ = printed_string_.find_first_of("%", ssp_);
		if (ssp_ != 0)
		{
			err = fprintf(strm, "%s", printed_string_.substr(0, ssp_).c_str());
		}

		auto offset = 2;
		auto fstr = printed_string_.substr(ssp_, offset);
		ssp_ += offset;

		err = fprintf(strm, fstr.c_str(), arg0);
		printed_arg0 = true;

		printed_string_ = printed_string_.substr(ssp_);
		ssp_ = 0;

		more_args_on_iter = _cpf_get_num_arg_specifiers(printed_string_, "%") > 0;
		if (!more_args_on_iter)
		{
			if (!printed_string_.empty())
			{
				err = fprintf(strm, "%s", printed_string_.c_str());
				printed_string_.clear();
			}
			std::advance(msd_iter, 1);
		}
	}
	else
	{
		ssp_ = 0;
		err = fprintf(strm, "%s", printed_string_.c_str());
		std::advance(msd_iter, 1);

		while (_cpf_get_num_arg_specifiers(msd_iter->second.second, "%") == 0)
		{
			_cpf_config_terminal(strm, msd_iter->second.first);
			err = fprintf(strm, "%s", msd_iter->second.second.c_str());
			std::advance(msd_iter, 1);
		}
	}

	bool iter_reached_end = (msd_iter == end_point_comparator);

	if (printed_arg0)
    {
		return  _cpf_call_(
			strm, end_point_comparator, msd_iter,
            (!more_args_on_iter && !iter_reached_end) ? msd_iter->second.second : printed_string_,
            more_args_on_iter, args...);
    }
	else
    {
		return  _cpf_call_(
			strm, end_point_comparator, msd_iter,
            (!more_args_on_iter && !iter_reached_end) ? msd_iter->second.second : printed_string_,
            ssp_, arg0, args...);
    }
}

/*
	descr
	*\

	args
	*\@

*/
template<typename... TN>
_cpf_types::error c_printf(	_cpf_types::stream strm, 
							const char* format, 
							const TN... args)
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

	err = _cpf_call_(	strm,
						m_iter_end_point_comparator,
						msd_iter_begin,
						msd_iter_begin->second.second,
						0,
						normalize_arg(args)...);
	return err;
}

