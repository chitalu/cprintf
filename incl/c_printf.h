#include "common.h"
#include "type_norm.h"
#include <stdexcept>
#include <iostream>

extern std::string g_current_colour_repr;
extern const std::vector<std::string> _cpf_colour_tokens;

extern void check_printf(const char* format);

template<class T, typename... Ts> void
check_printf(const char* format, const T& farg, const Ts&... args)
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

extern std::string _cpf_perform_block_space_parse(const std::string &src_format);

extern meta_format_t _cpf_perform_colour_token_parse(const std::string &formatter );

/*@stores the current system console text colour*/
extern "C" void _preserve_sys_attribs(void);

extern "C" void _recover_sys_attribs(void);

#ifdef _WIN32
extern void config_set_colour(stream_t stream, const std::string c_repr);
#else
extern std::string config_set_colour(const std::string c_repr);
#endif

template<class COUNTPRED>
_cpf_err_t _c_printf_(COUNTPRED c_pred,
	stream_t stream,
	const meta_format_t meta,
	meta_format_t::iterator &meta_iter,
	const std::string ostr,
	const std::size_t pos)
{
    int err = 0;
    if(meta_iter != meta.end())
    {
#ifdef _WIN32
        config_set_colour(stream, meta_iter->second.first);
        err = fprintf(stream, ostr.c_str(), arg0);
#else
        std::string colour_frmt_str = config_set_colour(meta_iter->second.first);
        auto prntd_str1 = (colour_frmt_str + ostr).c_str();
        err = fprintf(stream, "%s", prntd_str1);
#endif
    }
	_recover_sys_attribs();

	return 0;
}

/*replace predicate with actual function
change from passing const meta_format_t &meta
to meta.end()
*/
template<class COUNTPRED, typename T0, typename ...TN>
_cpf_err_t _c_printf_(	COUNTPRED c_pred,
				stream_t stream,
				const meta_format_t meta,
				meta_format_t::iterator &meta_iter,
			    const std::string ostr,
				const std::size_t pos,
				const T0 arg0,
			    const TN... args)
{
#ifdef _WIN32
	config_set_colour(stream, meta_iter->second.first);
#else
	std::string colour_frmt_str = config_set_colour(meta_iter->second.first);
#endif
	int err = 0;


	std::string _ostr = ostr;

	const auto argc = c_pred(ostr, "%");
	//more printf args to print in current meta format
	bool more_args = false;
	bool printed_arg0 = false;
    auto p_ = pos;

	if (argc >= 1)
	{
		p_ = _ostr.find_first_of("%", p_);
		if (p_ != 0)
		{
#ifdef _WIN32
			err = fprintf(stream, "%s", _ostr.substr(0, p_).c_str());
#else
			auto prntd_str = (colour_frmt_str + _ostr.substr(0, p_)).c_str();
			err = fprintf(	stream,
							"%s",
							prntd_str);
#endif
		}

		auto offset = 2;
		auto fstr = _ostr.substr(p_, offset);
		p_ += offset;
#ifdef _WIN32
		err = fprintf(stream, fstr.c_str(), arg0);
#else
		auto prntd_str1 = (colour_frmt_str + fstr).c_str();
		err = fprintf(stream, prntd_str1, arg0);
#endif
		printed_arg0 = true;

		_ostr = _ostr.substr(p_);
		p_ = 0;

		more_args = c_pred(_ostr, "%") > 0;
		if (!more_args)
		{
			if (!_ostr.empty())
			{
#ifdef _WIN32
				err = fprintf(stream, "%s", _ostr.c_str());
#else
				auto prntd_str2 = (colour_frmt_str + _ostr).c_str();
				err = fprintf(stream, "%s", prntd_str2);
#endif
				_ostr.clear();
			}
			meta_iter++;
		}
	}
	else
	{
		p_ = 0;
#ifdef _WIN32
		err = fprintf(stream, "%s", _ostr.c_str());
#else
		auto prntd_str2 = (colour_frmt_str + _ostr).c_str();
		err = fprintf(stream, "%s", prntd_str2);
#endif
		meta_iter++;
		while (c_pred(meta_iter->second.second, "%") == 0)
		{

#ifdef _WIN32
			config_set_colour(stream, meta_iter->second.first);
			err = fprintf(stream, "%s", meta_iter->second.second.c_str());
#else
			colour_frmt_str = config_set_colour(meta_iter->second.first);
			auto prntd_str3 = (colour_frmt_str + meta_iter->second.second).c_str();
			err = fprintf(stream, "%s", prntd_str3);
#endif
			meta_iter++;
		}
	}

	bool meta_iter_is_valid = (meta_iter != meta.end());

	if (printed_arg0)
    {
		return  _c_printf_(
                c_pred, stream, meta, meta_iter,
                (!more_args || meta_iter_is_valid) ? meta_iter->second.second : _ostr,
                p_, args...);
    }
	else
    {
		return  _c_printf_(
                c_pred, stream, meta, meta_iter,
                (!more_args && meta_iter_is_valid) ? meta_iter->second.second : _ostr,
                p_, arg0, args...);
    }
}

template<typename... TN>
_cpf_err_t c_printf(stream_t stream, const char* format, const TN... args)
{
	if (stream == nullptr){ throw std::runtime_error("output stream undefined");	}

	auto arg_count_pred = [=](const std::string & obj, const std::string & str) -> std::size_t
	{
		std::size_t n = 0;
		std::string::size_type pos = 0;
		while ((pos = obj.find(str, pos)) != std::string::npos)
		{
			n++;
			pos += str.size();
		}
		return n;
	};

#if defined(NDEBUG) || defined(_DEBUG)

	assert(
		(arg_count_pred(format, "%") > 0 && sizeof...(args) > 0) ||
		(arg_count_pred(format, "%") == 0 && sizeof...(args) == 0)
		);

	//check_printf(format, normalize_arg(args)...);


#endif

	auto g = _cpf_perform_block_space_parse(format);
    auto meta = _cpf_perform_colour_token_parse(g);
    //for (auto i : meta)
    //{
    //	std::cout << i.second.second << std::endl;
    //}
	//const std::size_t start_pos = 0;
    auto m_iter_begin = meta.begin();

	return _c_printf_(	arg_count_pred,
						stream,
						meta,
						m_iter_begin,
						meta.begin()->second.second,
						0,
						normalize_arg(args)...);

}

extern _cpf_err_t c_printf(FILE* stream, const char* format);
