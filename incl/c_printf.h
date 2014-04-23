#include "common.h"
#include "type_norm.h"
#include <stdexcept>

extern const std::vector<std::string> _g_col_reprs;

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

extern meta_format_t _parse_formatter(const char* _formatter);

/*@stores the current system console text colour*/
extern "C" void _save_sys_default_colour_(void);

extern "C" void _reload_sys_default_colour_(void);

#ifdef _WIN32
extern void _set_text_colour_(stream_t stream, const std::string& repr);
#else
extern std::string _set_text_colour_(const std::string& repr);
#endif

#ifdef _WIN32
extern void config_set_colour(stream_t stream, const std::string &c_repr);
#else
extern void config_set_colour(stream_t stream, const std::string &c_repr);
#endif

template<class COUNTPRED>
int _c_printf_(COUNTPRED c_pred,
	stream_t stream,
	meta_format_t &meta,
	meta_format_t::iterator &meta_iter,
	std::string &ostr,
	std::size_t &pos)
{	
	/*
		this function is executed last when processing arguments to printf
		so in order to get access to the last element of "meta" to get 
		the last set colour i use (std::end(meta) - 1)
	*/
	//config_set_colour(stream, (std::end(meta) - 1)->second.first);

	/*int err = 0;
	if (!ostr.empty())
	{
		err = fprintf(stream, ostr.c_str());
	}*/

	_reload_sys_default_colour_();

	return 0;
}

template<class COUNTPRED, typename T0, typename ...TN>
int _c_printf_(COUNTPRED c_pred,
			stream_t stream,
			meta_format_t &meta,
			meta_format_t::iterator &meta_iter, 
			std::string &ostr, 
			std::size_t &pos, 
			T0 arg0,
			TN... args)
{
	
	config_set_colour(stream, meta_iter->second.first);

	int err = 0;

	std::string _ostr = ostr.size() > 0 ? ostr : "";

	const auto argc = c_pred(ostr, "%");
	//more printf args to print in current meta format
	bool more_args = false;
	bool printed_arg0 = false;
	if (argc >= 1)
	{
		pos = ostr.find_first_of("%", pos);
		if (pos != 0)
		{
			err = fprintf(stream, ostr.substr(0, pos).c_str());
		}

		auto offset = 2;
		auto fstr = ostr.substr(pos, offset);
		pos += offset;

		err = fprintf(stream, fstr.c_str(), arg0);
		printed_arg0 = true;

		ostr = ostr.substr(pos);
		pos = 0;

		more_args = c_pred(ostr, "%") > 0;
		if (!more_args)
		{
			if (!ostr.empty())
			{
				fprintf(stream, ostr.c_str());
				ostr.clear();
			}
			meta_iter++;
		}
	}
	else
	{
		pos = 0;
		fprintf(stream, ostr.c_str());
		meta_iter++;
		while (c_pred(meta_iter->second.second, "%") == 0)
		{
			config_set_colour(stream, meta_iter->second.first);
			fprintf(stream, meta_iter->second.second.c_str());
			meta_iter++;
		}
	}

	bool meta_iter_is_valid = meta_iter != meta.end();

	if (printed_arg0)
		return _c_printf_(c_pred, stream, meta, meta_iter, (!more_args && meta_iter_is_valid) ? meta_iter->second.second : ostr, pos, args...);
	else
		return _c_printf_(c_pred, stream, meta, meta_iter, (!more_args && meta_iter_is_valid) ? meta_iter->second.second : ostr, pos, arg0, args...);
}

template<typename... TN> 
int c_printf(stream_t stream, const char* format, TN... args)
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

	auto meta = _parse_formatter(format);
	std::size_t start_pos = 0;
	
	return _c_printf_(	arg_count_pred, 
						stream, 
						meta, 
						meta.begin(), 
						meta.begin()->second.second, 
						start_pos, 
						normalize_arg(args)...);
}

extern int c_printf(FILE* stream, const char* format);