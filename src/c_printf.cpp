#include "c_printf.h"
#include <map>

#ifdef _WIN32

auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);

std::map<std::string, colour_t> g_colour_repr{ 
	{	"r",	(FOREGROUND_RED		| FOREGROUND_INTENSITY) },
	{	"g",	(FOREGROUND_GREEN	| FOREGROUND_INTENSITY) },
	{	"b",	(FOREGROUND_BLUE	| FOREGROUND_INTENSITY) },
	{	"a",	(FOREGROUND_BLUE	| FOREGROUND_GREEN) },
	{	"y",	(FOREGROUND_RED		| FOREGROUND_GREEN	| FOREGROUND_INTENSITY) },
	{	"m",	(FOREGROUND_RED		| FOREGROUND_BLUE	| FOREGROUND_INTENSITY) },
	{	"c",	(FOREGROUND_GREEN	| FOREGROUND_BLUE	| FOREGROUND_INTENSITY)	},
	{	"w",	(FOREGROUND_RED		| FOREGROUND_GREEN	| FOREGROUND_BLUE | FOREGROUND_INTENSITY) },
	{	"!",	[&]()->colour_t
				{
					CONSOLE_SCREEN_BUFFER_INFO csbi;
					GetConsoleScreenBufferInfo(console_stdout, &csbi);
					auto a = csbi.wAttributes;
					return static_cast<colour_t>(a % 16);
				}() 
	}
};

const colour_t SYS_TxCOL_UNDEFINED = 666;
colour_t g_sys_text_colour = SYS_TxCOL_UNDEFINED;

#else //_WIN32

std::map<std::string, colour_t> g_colour_repr{ 
	{	"r",	"\033[31m" },
	{	"g",	"\033[32m" },
	{	"b",	"\033[34m" },
	{	"a",	"\033[37m" },//TODO 
	{	"y",	"\033[33m" },
	{	"m",	"\033[35m" },
	{	"c",	"\033[36m" },
	{	"w",	"\033[37m" },
	{	"!",	"\033[37m" }//TODO
};

const colour_t SYS_TxCOL_UNDEFINED = "UNDEFINED";
colour_t g_sys_text_colour = SYS_TxCOL_UNDEFINED;

#endif //_WIN32

void check_printf(const char* format) 
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

meta_format_t _parse_formatter(const char* _formatter)
{
	auto extract_color_repr = [&](std::string fstr, std::size_t start_pos)->std::string
	{
		std::size_t offset, base;

		offset = base = (start_pos + 2);
		while (fstr[offset] != '}')	{	offset++; }
		assert(offset != fstr.npos);

		return fstr.substr(base, (offset - base));
	};
	
	meta_format_t meta;
	std::size_t pos;
	std::string subseq_colored_str, 
				color_repr, 
				formatter, 
				tag;

	formatter = _formatter;
	tag = "${";

	pos = formatter.find(tag, 0);

	if (pos == formatter.npos)
	{
		meta.push_back(std::make_pair(std::string("!"), formatter));
		return meta;
	}
	else
	{
		meta.push_back(std::make_pair(std::string("!"), formatter.substr(0, pos)));
	}

	color_repr = extract_color_repr(formatter, pos);
	/*plus 1 is for ")", to complete the symbolic color tag's size i.e ${s}*/
	pos += (tag.size() + (color_repr.size()) + 1);

	auto _p = formatter.find(tag, pos);
	if (_p != formatter.npos){	_p -= pos;	}

	auto offpos_insert_meta = [&](std::size_t count)->void
	{
		subseq_colored_str = formatter.substr(pos, count);
		meta.push_back(std::make_pair(color_repr, subseq_colored_str));
	};

	offpos_insert_meta(_p);

	while (pos != std::string::npos)
	{
		pos = formatter.find(tag, pos + 1);
		color_repr = extract_color_repr(formatter, pos);
		pos += (tag.size() + (color_repr.size()) + 1);

		_p = formatter.find(tag, pos);

		if (_p >= formatter.npos)
		{
			offpos_insert_meta(formatter.npos);		
			break;
		}

		if (_p != formatter.npos){	_p -= pos;	}

		offpos_insert_meta(_p);
	}

	return meta;
}

int c_printf(stream_t stream, const char* format)
{
	return 0;
}

void _save_sys_default_colour_(void)
{
	if (g_sys_text_colour == SYS_TxCOL_UNDEFINED)
	{
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console_stdout, &csbi);
		auto a = csbi.wAttributes;
		g_sys_text_colour = static_cast<colour_t>(a % 16);
#else
		/*TODO:*/
#endif
	}
}

void _reload_sys_default_colour_(void)
{
#ifdef _WIN32
	for (auto &handle : {console_stdout, console_stderr})
	{
		SetConsoleTextAttribute(handle, g_sys_text_colour);
	}
#else

#endif
}

#ifdef _WIN32
extern void _set_text_colour_(stream_t stream, const std::string& repr)
{
	if (stream == stdout)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), g_colour_repr[repr]);
	}
	else if (stream == stderr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), g_colour_repr[repr]);
	}
}

std::string g_current_colour_repr = "666";

void config_set_colour(stream_t stream, const std::string &c_repr)
{
	if (g_current_colour_repr.compare(c_repr) != 0)
	{
		g_current_colour_repr = c_repr;
	}

	_set_text_colour_(stream, g_current_colour_repr);
}

#else
extern std::string _set_text_colour_(const std::string& repr)
{
	
}

void config_set_colour(stream_t stream, const std::string &c_repr)
{
	
}
#endif