#include "common.h"
#include "type_norm.h"
#include <stdexcept>
#include <iostream>

extern _cpf_types::_string_type_ g_current_colour_repr;
extern const _cpf_types::string_vector _cpf_colour_tokens;

extern void _cpf_authenticate_format_string(const char* format);

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
			printf("f\n");
			break;
		case 'd':
			assert(std::is_integral<T>::value);
			printf("d\n");
			break;
		case 'c':
			assert(std::is_integral<T>::value);
			printf("c\n");
			break;
		case 's':
			assert(std::is_pointer<T>::value);
			printf("s\n");
			break;
		default:
			printf("warning: unsupported formatter: %c\n", f);
			break;
		}
		return check_printf(++format, args...);
	}

	throw std::invalid_argument("Too few format specifiers.");
}

extern _cpf_types::_string_type_ _cpf_do_block_space_parse(
	const _cpf_types::_string_type_ &src_format);

extern _cpf_types::meta_format_type _cpf_do_colour_token_parse(
	const _cpf_types::_string_type_ &formatter );

/*@stores the current system console text colour*/
extern "C" void _preserve_sys_attribs(void);

extern "C" void _recover_sys_attribs(void);

extern void config_set_colour(	_cpf_types::stream strm, 
								const _cpf_types::_string_type_ c_repr);

extern std::size_t _cpf_get_num_arg_specifiers(	const _cpf_types::_string_type_ & obj, 
												const _cpf_types::_string_type_ & str);

extern _cpf_types::error _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::iterator &end_point_comparator,
	_cpf_types::meta_format_type::iterator &meta_iter,
	const _cpf_types::_string_type_ ostr,
	const std::size_t pos);

/*TODO: replace predicate with actual function
change from passing const meta_format_t &meta
to meta.end()
*/
template<typename T0, typename ...TN>
_cpf_types::error _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::iterator &end_point_comparator,
	_cpf_types::meta_format_type::iterator &meta_iter,
    const _cpf_types::_string_type_ ostr,
	const std::size_t pos,
	const T0 arg0,
    const TN... args)
{
	config_set_colour(strm, meta_iter->second.first);

	_cpf_types::error err = 0;

	_cpf_types::_string_type_ _ostr = ostr;

	const auto argc = _cpf_get_num_arg_specifiers(ostr, "%");
	//more printf args to print in current meta format
	bool more_args = false;
	bool printed_arg0 = false;
    auto p_ = pos;

	if (argc >= 1)
	{
		p_ = _ostr.find_first_of("%", p_);
		if (p_ != 0)
		{
			err = fprintf(strm, "%s", _ostr.substr(0, p_).c_str());
		}

		auto offset = 2;
		auto fstr = _ostr.substr(p_, offset);
		p_ += offset;

		err = fprintf(strm, fstr.c_str(), arg0);
		printed_arg0 = true;

		_ostr = _ostr.substr(p_);
		p_ = 0;

		more_args = _cpf_get_num_arg_specifiers(_ostr, "%") > 0;
		if (!more_args)
		{
			if (!_ostr.empty())
			{
				err = fprintf(strm, "%s", _ostr.c_str());
				_ostr.clear();
			}
			meta_iter++;
		}
	}
	else
	{
		p_ = 0;
		err = fprintf(strm, "%s", _ostr.c_str());
		meta_iter++;
		while (_cpf_get_num_arg_specifiers(meta_iter->second.second, "%") == 0)
		{
			config_set_colour(strm, meta_iter->second.first);
			err = fprintf(strm, "%s", meta_iter->second.second.c_str());
			meta_iter++;
		}
	}

	bool meta_iter_is_valid = (meta_iter != end_point_comparator);

	if (printed_arg0)
    {
		return  _cpf_call_(strm, end_point_comparator, meta_iter,
                (!more_args && meta_iter_is_valid) ? meta_iter->second.second : _ostr,
                p_, args...);
    }
	else
    {
		return  _cpf_call_(strm, end_point_comparator, meta_iter,
                (!more_args && meta_iter_is_valid) ? meta_iter->second.second : _ostr,
                p_, arg0, args...);
    }
}

template<typename... TN>
_cpf_types::error c_printf(	_cpf_types::stream strm, 
							const char* format, 
							const TN... args)
{
	_cpf_types::error err = 0;

	if (strm == nullptr)
	{ 
		throw std::runtime_error("output stream undefined");
	}
	else if(format == nullptr)
	{
		throw std::runtime_error("format undefined");
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
    
    auto msd_iter_begin = meta_string_data.begin();
    auto m_iter_end_point_comparator = meta_string_data.end();

	err = _cpf_call_(	strm,
						m_iter_end_point_comparator,
						msd_iter_begin,
						msd_iter_begin->second.second,
						0,
						normalize_arg(args)...);
	return err;
}

