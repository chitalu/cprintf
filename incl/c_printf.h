#include "common.h"

#include <type_traits>

template<class T> 
typename std::enable_if<std::is_integral<T>::value, long>::type
normalize_arg(T arg) 
{
	return arg;
}

template<class T> 
typename std::enable_if<std::is_floating_point<T>::value, double>::type
normalize_arg(T arg) 
{
	return arg;
}

template<class T> 
typename std::enable_if<std::is_pointer<T>::value, T>::type
normalize_arg(T arg) 
{
	return arg;
}

extern const char* normalize_arg(const std::string& arg);

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

template<class COUNTPRED, typename ...TN>
void _c_printf_(COUNTPRED c_pred,
				FILE* stream,
				std::vector<std::pair<std::string, std::string>> &meta,
				std::vector<std::pair<std::string, std::string>>::iterator &meta_iter,
				std::string &ostr,
				std::size_t &pos)
{	}

template<class COUNTPRED, typename ...TN>
void _c_printf_(COUNTPRED c_pred,
			FILE* stream,
			meta_format_t &meta,
			meta_format_t::iterator &meta_iter, 
			std::string &ostr, 
			std::size_t &pos, 
			TN... args)
{
	int err = 0;
	if (meta_iter == meta.end())
	{
		return;
	}

	std::string _ostr = ostr.size() > 0 ? ostr : "";

	const auto argc = c_pred(ostr, "%");

	if (argc >= 1)
	{
		pos = ostr.find_first_of("%", pos);
		if (pos != 0)
		{
			err = fprintf(stream, ostr.substr(0, pos).c_str());
		}

		auto fstr = ostr.substr(pos, pos + 1);
		pos++;

		auto argval = std::get<0>(std::make_tuple(args...));
		err = fprintf(stream, fstr.c_str(), argval);

		_ostr = ostr.substr(pos);

		bool more = c_pred(_ostr, "%") > 0;
		if (!more)
		{
			pos = 0;
			meta_iter++;
		}

		_c_printf_(c_pred, stream, meta, meta_iter, (!more) ? meta_iter->second : _ostr, pos, args...);
	}
	else
	{
		pos = 0;
		meta_iter++;
		fprintf(stream, ostr.c_str());
		_c_printf_(c_pred, stream, meta, meta_iter, _ostr, pos, args...);
	}
}

template<typename... TN> 
int c_printf(FILE* stream, const char* format, TN... args)
{ 
	auto count = [=](const std::string & obj, const std::string & str) -> std::size_t
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
		(count(format, "%") > 0 && sizeof...(args) > 0) ||
		(count(format, "%") == 0 && sizeof...(args) == 0)
		);

	//check_printf(format, normalize_arg(args)...);


#endif

	auto meta = _parse_formatter(format);
	const auto total_argc = count(format, "%");
	std::size_t start_pos = 0;

	_c_printf_(count, stream, meta, meta.begin(), meta.begin()->second, start_pos, normalize_arg(args)...);
	
	return 0;// _c_printf_(stream, format, normalize_arg(args)...);
}

extern int c_printf(FILE* stream, const char* format);