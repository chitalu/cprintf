#include "c_printf.h"
#include <stdlib.h>     /* atoi */
#include <algorithm>

colour_t _cpf_sys_attribs = S_T_A_UNDEF;

#ifdef _WIN32

auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);

#else //_WIN32
//http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
//http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
//http://linuxgazette.net/issue65/padala.html
/*const std::map<const std::string, colour_t> _cpf_colour_token_vals{ 
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
};*/

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

std::string _cpf_perform_block_space_parse(std::string src_format)
{
	std::string output;
	std::string bs_tag = "${|";
	auto x = 0;
	std::string::size_type pos = 0;
	bool first_iter = true;
	while ((pos = src_format.find(bs_tag, x)) != src_format.npos)
	{
		if (pos != 0 && first_iter)
		{	
			first_iter = false;
			output.append(src_format.substr(0, pos));
		}

		x = src_format.find_first_of('}', pos);
		int i = pos + bs_tag.size();
		std::string blk_size, s;
		s = src_format.substr(i, x - i);
		bool bright = false;
		int _p = 1;
		std::string lst{ *(s.end() - _p) };
		std::string colour = lst;
		colour.resize(2, colour[0]);
		if (lst == "!")
		{
			_p = 2;
			bright = true;
			colour = *(s.end() - _p) ;
			colour.append("!");

			colour += colour;
		}

		blk_size = src_format.substr(
			i,
			std::distance(s.begin(), (s.end() - _p))
			);
			
		auto rblk_sze = atoi(blk_size.c_str());
		std::string s_;
		s_.resize(rblk_sze, '-');
		output.append("${" + colour + "}" + s_ + "${!}");

		auto t = src_format.find(bs_tag, x + 1);
		auto t_ = src_format.substr(x + 1, t);
		output.append(t_);
	}
	return output;
}

meta_format_t _cpf_perform_colour_token_parse(const char* _formatter)
{
	meta_format_t meta;
	
	std::string formatter, 
		_c_prefix, _c_suffix;

	formatter = _formatter,
	_c_prefix = "${",
	_c_suffix = "}";

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		std::string::size_type start = 0;

		while ((start = formatter.find(_c_prefix, start)) != std::string::npos) 
		{
			++occurrences;
			start += _c_prefix.length();
		}
		return occurrences;
	}();

	auto first_c_frmt_pos = formatter.find(_c_prefix);
	if (first_c_frmt_pos != 0)
	{
		meta.insert(
			std::make_pair(0, std::make_pair("!", formatter.substr(0, first_c_frmt_pos)))
			);
	}

	std::size_t counter = 0;
	for (auto &c_repr : _cpf_colour_tokens)
	{
		if (counter > NUM_C_TAGS)
		{
			break;
		}
		auto c_frmt = _c_prefix + c_repr + _c_suffix;
		auto pos = formatter.find(c_frmt);

		while (pos != formatter.npos)
		{
			auto p_ = pos;
			auto p_offset = p_ + c_frmt.length();
			pos = formatter.find(_c_prefix, p_offset);
			auto cf = formatter.substr(p_, c_frmt.length());

			if (cf == c_frmt)
			{
				meta.insert(
					std::make_pair(p_offset, std::make_pair(c_repr, formatter.substr(p_offset, (pos - p_offset))))
					);
				pos = formatter.find(c_frmt, p_offset);
				++counter;
			}
		}
	}

	return meta;
}

int c_printf(stream_t stream, const char* format)
{
	return 0;
}

void _preserve_sys_attribs(void)
{
	if (_cpf_sys_attribs == S_T_A_UNDEF)
	{
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console_stdout, &csbi);
		auto a = csbi.wAttributes;
		_cpf_sys_attribs = static_cast<colour_t>(a % 16);
#else
		/*TODO:*/
#endif
	}
}

void _recover_sys_attribs(void)
{
#ifdef _WIN32
	for (auto &handle : {console_stdout, console_stderr})
	{
		SetConsoleTextAttribute(handle, _cpf_sys_attribs);
	}
#else

#endif
}

#ifdef _WIN32
extern void _set_text_colour_(stream_t stream, const std::string& repr)
{
	HANDLE h;//test this for mem leaks
	if (stream == stdout)
	{
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else if (stream == stderr)
	{
		h = GetStdHandle(STD_ERROR_HANDLE);
	}

	SetConsoleTextAttribute(h, _cpf_colour_token_vals.find(repr)->second);
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
	return std::string();
}

void config_set_colour(stream_t stream, const std::string &c_repr)
{
	
}
#endif
