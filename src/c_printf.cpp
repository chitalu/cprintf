#include "c_printf.h"
#include <map>

const std::vector<std::string> _g_col_reprs = {
	/*dim text colour no background*/
	"r", "g", "b", 
	/*bright text colour no background*/
	"r!", "g!", "b!",
	/*dim text and background colour*/
	"rr", "rb", "rg",	/*red*/
	"gg", "gb", "gr",	/*green*/
	"bb", "br", "bg",	/*blue*/
	
	/*bright text colour and dim background colour*/
	"r!r", "r!b", "r!g", /*red*/
	"g!g", "g!b", "g!r", /*green*/
	"b!b", "b!r", "b!g", /*blue*/
	
	/*bright text and background colour*/
	"r!r!", "r!b!", "r!g!", /*red*/
	"g!g!", "g!b!", "g!r!", /*green*/
	"b!b!", "b!r!", "b!g!", /*blue*/
	
	/*system console text colour prior to c_printf call (default)*/
	"!" 
};

#ifdef _WIN32

auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);

std::map<std::string, colour_t> g_colour_repr{ 
	{	"r",	(FOREGROUND_RED)	},
	{	"rb",	(FOREGROUND_RED | BACKGROUND_BLUE) },
	{	"r!b",	((FOREGROUND_RED | FOREGROUND_INTENSITY) | BACKGROUND_BLUE) },
	{	"r!b!", ((FOREGROUND_RED | FOREGROUND_INTENSITY) | (BACKGROUND_BLUE | BACKGROUND_INTENSITY)) },
	{	"g",	(FOREGROUND_GREEN)	},
	{	"b",	(FOREGROUND_BLUE)	},
	{	"r!",	(FOREGROUND_RED		| FOREGROUND_INTENSITY) },
	{	"g!",	(FOREGROUND_GREEN	| FOREGROUND_INTENSITY) },
	{	"b!",	(FOREGROUND_BLUE	| FOREGROUND_INTENSITY) },
	/*{	"y",	(FOREGROUND_RED		| FOREGROUND_GREEN	| FOREGROUND_INTENSITY) },
	{	"m",	(FOREGROUND_RED		| FOREGROUND_BLUE	| FOREGROUND_INTENSITY) },
	{	"c",	(FOREGROUND_GREEN	| FOREGROUND_BLUE	| FOREGROUND_INTENSITY)	},
	{	"w",	(FOREGROUND_RED		| FOREGROUND_GREEN	| FOREGROUND_BLUE | FOREGROUND_INTENSITY) },*/
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
//http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
//http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
//http://linuxgazette.net/issue65/padala.html
std::map<std::string, colour_t> g_colour_repr{ 
	{	"r",	"\033[31m" },
	{	"g",	"\033[32m" },
	{	"b",	"\033[34m" },
	{	"r!",	"\033[31m" },
	{	"g!",	"\033[32m" },
	{	"b!",	"\033[34m" },
	//{	"a",	"\033[37m" },//TODO 
	//{	"y",	"\033[33m" },
	//{	"m",	"\033[35m" },
	//{	"c",	"\033[36m" },
	//{	"w",	"\033[37m" },
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
	
	std::string formatter, 
				tag;

	formatter = _formatter;
	tag = "|";
	auto first_c_frmt_pos = formatter.find(tag);
	if (first_c_frmt_pos != 0)
	{
		meta.insert(
			std::make_pair(0, std::make_pair("!", formatter.substr(0, first_c_frmt_pos)))
			);
	}

	for (auto &c_repr : _g_col_reprs)
	{
		auto c_frmt = tag + c_repr + tag;
		auto pos = formatter.find(c_frmt);

		while (pos != formatter.npos)
		{
			auto p_ = pos;
			auto p_offset = p_ + c_frmt.length();
			pos = formatter.find("|", p_offset);
			auto cf = formatter.substr(p_, c_frmt.length());
			if (cf == c_frmt)
			{
				meta.insert(
					std::make_pair(p_offset, std::make_pair(c_repr, formatter.substr(p_offset, (pos - p_offset))))
					);
				pos = formatter.find(c_frmt, p_offset);
			}
			
		}
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