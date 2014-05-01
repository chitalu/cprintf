#include "common.h"
#include "type_norm.h"
#include <stdexcept>

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

extern std::string _cpf_perform_block_space_parse(std::string src_format);

extern meta_format_t _cpf_perform_colour_token_parse(const char* _formatter);

/*@stores the current system console text colour*/
extern "C" void _preserve_sys_attribs(void);

extern "C" void _recover_sys_attribs(void);

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
	meta_format_t::iterator meta_iter,
	const std::string ostr,
	const std::size_t pos)
{	

	_recover_sys_attribs();

	return 0;
}

template<class COUNTPRED, typename T0, typename ...TN>
int _c_printf_(COUNTPRED c_pred,
			stream_t stream,
			meta_format_t &meta,
			meta_format_t::iterator meta_iter, 
		    const std::string ostr, 
			const std::size_t pos, 
			const T0 arg0,
		    const TN... args)
{
	
	config_set_colour(stream, meta_iter->second.first);

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
			err = fprintf(stream, "%s", _ostr.substr(0, p_).c_str());
		}

		auto offset = 2;
		auto fstr = _ostr.substr(p_, offset);
		p_ += offset;

		err = fprintf(stream, fstr.c_str(), arg0);
		printed_arg0 = true;

		_ostr = _ostr.substr(p_);
		p_ = 0;

		more_args = c_pred(_ostr, "%") > 0;
		if (!more_args)
		{
			if (!_ostr.empty())
			{
				fprintf(stream, "%s", _ostr.c_str());
				_ostr.clear();
			}
			meta_iter++;
		}
	}
	else
	{
		p_ = 0;
		fprintf(stream, "%s", _ostr.c_str());
		meta_iter++;
		while (c_pred(meta_iter->second.second, "%") == 0)
		{
			config_set_colour(stream, meta_iter->second.first);
			fprintf(stream, "%s", meta_iter->second.second.c_str());
			meta_iter++;
		}
	}

	bool meta_iter_is_valid = meta_iter != meta.end();

	if (printed_arg0)
    {
		return  _c_printf_(  
                c_pred, stream, meta, meta_iter, 
                (!more_args && meta_iter_is_valid) ? meta_iter->second.second : _ostr, 
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
int c_printf(stream_t stream, const char* format, const TN... args)
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
    auto meta = _cpf_perform_colour_token_parse(g.c_str());
	//const std::size_t start_pos = 0;
    	
	return _c_printf_(	arg_count_pred, 
						stream, 
						meta, 
						meta.begin(), 
						meta.begin()->second.second, 
						0, 
						normalize_arg(args)...);
}

extern int c_printf(FILE* stream, const char* format);
