/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

*/

#include <wchar.h> //wcstol
#include "_cpf_config.h"

#if _WIN32

#include <conio.h> // _getch

HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

#define CONFIG_FG (0)
#define CONFIG_BG (1)
#define CONFIG_FGBG (2)

#else
#include <termios.h>
#include <unistd.h>
#endif

cpf::type::attribute_group _cpf_current_text_attribs;
static bool glob_terminal_state_restored = true;

#ifdef _WIN32
static cpf::type::colour saved_terminal_colour;
#endif

void cpf::intern::save_stream_state(cpf::type::stream user_stream)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	auto s = user_stream == stdout ? stdout_handle : stderr_handle;
	auto ret_okay = GetConsoleScreenBufferInfo(s, &cbsi);

	if (!ret_okay)
	{
		throw cpf::type::except(L"CPF-RT-ERR: failed to retrieve terminal attributes");
	}
	saved_terminal_colour = cbsi.wAttributes;
#else
	
#endif
	glob_terminal_state_restored = false;
}

void cpf::intern::restore_stream_state(cpf::type::stream user_stream, bool finished_cpf_exec)
{
	if (!glob_terminal_state_restored)
	{
#ifdef _WIN32
		auto s = user_stream == stdout ? stdout_handle : stderr_handle;
		auto ret_okay = SetConsoleTextAttribute(s, saved_terminal_colour);
		if (!ret_okay)
		{
			throw cpf::type::except(L"CPF-RT-ERR: failed to restore terminal attributes");
		}
#else
		fwprintf(user_stream, L"\x1B[0;0;0m");
#endif
		/*ternary op guarrantees that console settings will be reset to 
		originals i.e as they were prior to calling a cprintf function*/
		glob_terminal_state_restored = finished_cpf_exec ? true : false;
	}
}

bool _cpf_is_fstream(cpf::type::stream user_stream)
{
	bool is_fstream = true;
	for (auto s : { stdout, stderr })
	{
		if (user_stream == s)
		{
			is_fstream = false;
			break;
		}
	}
	return is_fstream;
}

//i.e 32f or 200b
bool is_bitmap_colour_token(const cpf::type::str& attrib)
{
	for (auto c = std::begin(attrib); c != std::end(attrib); ++c)
	{
		//if any value in the attribute is a digit
		if (isdigit(*c))
		{
			return true;
		}
	}
	return false;
}

bool is_cursor_pos_attrib(const cpf::type::str& attrib)
{
	auto asize = attrib.size();
	auto num_commas = std::count(attrib.begin(), attrib.end(), ',');

	bool value = isdigit(attrib[0]) && num_commas != 0;

	if (value)
	{
		if (num_commas != 1)
		{
			throw cpf::type::except(cpf::type::str(L"CPF-RT-ERR: invalid cursor position specifier: " + attrib).c_str());
		}

		for (auto c = std::begin(attrib); c != std::end(attrib); ++c)
		{
			//if any value in the attribute is not a digit and its not a comma 
			if (!isdigit(*c) && *c != ',')
			{
				throw cpf::type::except(
					cpf::type::str(L"CPF-RT-ERR: invalid character in cursor position specifier: " + attrib).c_str());
			}
		}
	}

	return value;
}

cpf::type::colour get_token_value(const cpf::type::str& colour_key)
{
	auto terminal_value = cpf::std_token_vals.find(colour_key);
	if (terminal_value == cpf::std_token_vals.end())
	{
		throw cpf::type::except((cpf::type::str(L"CPF-RT-ERR: invalid token : ") + colour_key).c_str());
	}
	return terminal_value->second;
}

#ifdef __gnu_linux__
cpf::type::str get_terminal_bitmap_colour_value(const cpf::type::str& attrib_token)
{
	auto at_size = attrib_token.size();
	wchar_t lst_char = attrib_token[at_size - 1];
	auto colour_num = attrib_token.substr(0, at_size - 2);

	auto int_repr = wcstol(colour_num.c_str(), nullptr, 0);

	if ((lst_char != 'f' && lst_char != 'b' && lst_char != '&') || at_size == 1 || (int_repr > 256 || int_repr < 0))
	{
		throw cpf::type::except(cpf::type::str(L"CPF-RT-ERR: invalid attribute token: ").append(attrib_token).c_str());
	}

	cpf::type::str colour_str;

	if (lst_char == 'f')//foreground
	{
		colour_str = (L"\x1B[38;5;" + colour_num + L"m");
	}
	else if (lst_char == 'b') //background
	{
		colour_str = (L"\x1B[48;5;" + colour_num + L"m");
	}
	else // "43&"
	{
		colour_str = (L"\x1B[38;5;" + colour_num + L"m\x1B[48;5;" + colour_num + L"m");
	}

	return colour_str;
}
#endif



void set_cursor_position(cpf::type::stream user_stream, const cpf::type::str& attrib_str)
{
	auto comma_pos = attrib_str.find(',');
	auto horizontal_pos_str = attrib_str.substr(0, comma_pos);
	auto vertical_pos_str = attrib_str.substr(comma_pos + 1);

#ifdef _WIN32
	auto horizontal_pos = wcstol(horizontal_pos_str.c_str(), nullptr, 0);
	auto vertical_pos = wcstol(vertical_pos_str.c_str(), nullptr, 0);

	COORD coords;
	coords.X = static_cast<SHORT>(horizontal_pos);
	coords.Y= static_cast<SHORT>(vertical_pos);
	BOOL result = SetConsoleCursorPosition(
		user_stream == stdout ? stdout_handle : stderr_handle, 
		coords);

	if (!result)
	{
		throw cpf::type::except(L"CPF-RT-ERR: invalid coordinates");
	}
#else
	//http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
	fwprintf(user_stream, L"%s", (L"\x1B[" + vertical_pos_str + L";" + horizontal_pos_str+ L"H").c_str());
#endif
}

void clear_terminal(	cpf::type::stream user_stream,
							const cpf::type::str& attrib)
{
#ifdef _WIN32
	auto user_stream_ = user_stream == stdout ? stdout_handle : stderr_handle;
	COORD coord = {0, 0};

	auto f = [&]()
	{
		DWORD count;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(user_stream_, &csbi);

		/*
		Sets the character attributes for a specified number of character cells, 
		beginning at the specified coordinates in a screen buffer.
		*/
		FillConsoleOutputAttribute(user_stream_, 
			saved_terminal_colour,
			csbi.dwSize.X * csbi.dwSize.Y,
			coord,
			&count);

		/*
		Writes a character to the console screen buffer a specified number of times, 
		beginning at the specified coordinates.
		*/
		FillConsoleOutputCharacter(user_stream_,
			' ',
			csbi.dwSize.X * csbi.dwSize.Y,
			coord,
			&count);
	};
	
	if (attrib == L"!")
	{
		f();
		SetConsoleCursorPosition(user_stream_, coord);
	}
	else // "!~"
	{
		f();
	}

#else
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	if(attrib == L"!")
	{
		fwprintf(user_stream, L"\x1B[2J\x1B[H");
	}
	
	if (attrib == L"!~")
	{
		fwprintf(user_stream, L"\x1B[2J");
	}
#endif
}

#ifndef _WIN32
/*
	This code sets the terminal into non-canonical mode, thus disabling line buffering,
	reads a character from stdin and then restores the old terminal status. For more 
	info on what else you can do with termios, see ``man termios''
*/
int _getch(void) 
{
	struct termios	oldt,
					newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

#endif

void config_text_attribute(	cpf::type::stream user_stream, 
							const cpf::type::colour &user_colour, 
							std::uint8_t col_config_type = 255)
{
#ifdef _WIN32

	/*
	interesting: 
	http://comp.os.ms-windows.programmer.win32.narkive.com/1bOxy0qZ/extended-attributes-all-broken-console-api
	*/
	auto output_stream_handle = user_stream == stdout ? stdout_handle : stderr_handle;

	CONSOLE_SCREEN_BUFFER_INFOEX cbie;
	cbie.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(output_stream_handle, &cbie); //get info

	switch (col_config_type)
	{
	case CONFIG_FG:
		/*
			we must first cancel out all foreground text attributes
			then set foreground attribute to user-specified colour.
		*/
		cbie.wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cbie.wAttributes |= (user_colour);

		SetConsoleTextAttribute(output_stream_handle, cbie.wAttributes);
		break;
	case CONFIG_BG:
		cbie.wAttributes &= ~(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		cbie.wAttributes |= (user_colour);
		SetConsoleTextAttribute(output_stream_handle, cbie.wAttributes);
		break;
	case CONFIG_FGBG:
		cbie.wAttributes = user_colour;
		SetConsoleTextAttribute(output_stream_handle, user_colour);
		break;
	}
#else

#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
	/*colour is controled via Control Sequences*/
	fwprintf(user_stream, user_colour.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif

#endif
}

CPF_API void cpf::intern::configure(cpf::type::stream user_stream,
									const cpf::type::attribute_group& attribute_group)
{
	if (_cpf_is_fstream(user_stream))
	{
		return; //no configurations necessary if writing to file
	}

	if (_cpf_current_text_attribs != attribute_group)
	{
		_cpf_current_text_attribs = attribute_group;

		for (auto a = std::begin(attribute_group); a != std::end(attribute_group); ++a)
		{
			auto tok = *a;

			if (is_cursor_pos_attrib(tok))
			{
				set_cursor_position(user_stream, tok);
			}
			else if (tok == L"|") /*halt until input*/
			{
				_getch();
			}
			else if (tok == L"!" || tok == L"!~") /*clear screen*/
			{
				clear_terminal(user_stream, tok);
			}
			else if (tok == L"?") /*restore colour to system default*/
			{
				cpf::intern::restore_stream_state(user_stream);
			}
			else /*set colour attribute(s)*/
			{
				auto is_bmct = is_bitmap_colour_token(tok);
#ifdef _WIN32
				if (is_bmct)
				{
					//because windows does not support that.
					throw cpf::type::except((cpf::type::str(L"CPF-RT-ERR: invalid token : ") + tok).c_str());
				}

				std::uint8_t config_type;

				/*configuration type is needed to determine which bitwise
				operations to do on setting colour values in function config_text_attribute*/
				if (tok.size() == 1 || (tok.size() == 2 && tok[1] == '*'))
				{
					config_type = CONFIG_FG;
				}
				else if (tok.size() >= 2 && tok.size() <= 4 && tok[tok.size() - 1] != '#')
				{
					config_type = CONFIG_FGBG;
				}
				else if (tok[tok.size()-1] == '#')
				{
					config_type = CONFIG_BG;
				}
				else
				{
					throw cpf::type::except((cpf::type::str(L"CPF-RT-ERR: invalid token : ") + tok).c_str());
				}
				
				cpf::type::colour colour_value = get_token_value(tok);
				config_text_attribute(user_stream, colour_value, config_type);
				
#else
				cpf::type::str control_sequence;
				if(is_bmct)
				{
					control_sequence = get_terminal_bitmap_colour_value(tok);
				}
				else
				{
					control_sequence = get_token_value(tok);
				}

				config_text_attribute(user_stream, control_sequence);
#endif
			}/*is_cursor_pos_attrib*/
		}
	}
}