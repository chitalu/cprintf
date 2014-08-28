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

#include "_cpf_config.h"

#if _WIN32

#include <conio.h> // _getch

HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

#else
#include <termios.h>
#include <unistd.h>
#endif

_cpf_type::attribs _cpf_current_text_attribs;
static bool glob_terminal_state_restored = true;

#ifdef _WIN32
static _cpf_type::colour saved_terminal_colour;
#endif

void save_terminal_settings(_cpf_type::stream user_stream)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	auto s = user_stream == stdout ? stdout_handle : stderr_handle;
	auto ret_okay = GetConsoleScreenBufferInfo(s, &cbsi);

	if (!ret_okay)
	{
		throw _cpf_type::error("cpf fatal error: failed to retrieve terminal attributes");
	}
	saved_terminal_colour = cbsi.wAttributes;
#else
	
#endif
	glob_terminal_state_restored = false;
}

void restore_terminal_settings(_cpf_type::stream user_stream, bool finished_cpf_exec)
{
	if (!glob_terminal_state_restored)
	{
#ifdef _WIN32
		auto s = user_stream == stdout ? stdout_handle : stderr_handle;
		auto ret_okay = SetConsoleTextAttribute(s, saved_terminal_colour);
		if (!ret_okay)
		{
			throw _cpf_type::error("cpf fatal error: failed to restore terminal attributes");
		}
#else
		fprintf(user_stream, "\x1B[0;0;0m");
#endif
		/*ternary op guarrantees that console settings will be reset to 
		originals i.e as they were prior to calling a cprintf function*/
		glob_terminal_state_restored = finished_cpf_exec ? true : false;
	}
}

bool _cpf_is_fstream(_cpf_type::stream user_stream)
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
bool is_bitmap_colour_token(const _cpf_type::str& attrib)
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

bool is_cursor_pos_attrib(const _cpf_type::str& attrib)
{
	auto asize = attrib.size();
	auto num_commas = std::count(attrib.begin(), attrib.end(), ',');

	bool value = isdigit(attrib[0]) && num_commas != 0;

	if (value)
	{
		if (num_commas != 1)
		{
			throw _cpf_type::error(std::string("invalid cursor position specifier: " + attrib).c_str());
		}

		for (auto c = std::begin(attrib); c != std::end(attrib); ++c)
		{
			//if any value in the attribute is not a digit and its not a comma 
			if (!isdigit(*c) && *c != ',')
			{
				throw _cpf_type::error(
					std::string("invalid character in cursor position specifier: " + attrib).c_str());
			}
		}
	}

	return value;
}

_cpf_type::colour safely_get_terminal_value(const _cpf_type::str& colour_key)
{
	auto terminal_value = _cpf_std_token_vals.find(colour_key);
	if (terminal_value == _cpf_std_token_vals.end())
	{
		throw _cpf_type::error(_cpf_type::str("invalid attribute token: ").append(colour_key).c_str());
	}
	return terminal_value->second;
}

#ifdef __gnu_linux__
_cpf_type::str get_terminal_bitmap_colour_value(const _cpf_type::str& attrib_token)
{
	auto at_size = attrib_token.size();
	char lst_char = attrib_token[at_size - 1];
	auto colour_num = attrib_token.substr(0, at_size - 2);

	auto int_repr = atoi(colour_num.c_str());
	if ((lst_char != 'f' && lst_char != 'b' && lst_char != '&') || at_size == 1 || (int_repr > 256 || int_repr < 0))
	{
		throw _cpf_err(_cpf_type::str("invalid attribute token: ").append(attrib_token).c_str());
	}

	_cpf_type::str colour_str;

	if (lst_char == 'f')//foreground
	{
		colour_str = ("\x1B[38;5;" + colour_num + "m");
	}
	else if (lst_char == 'b') //background
	{
		colour_str = ("\x1B[48;5;" + colour_num + "m");
	}
	else // "43&"
	{
		colour_str = ("\x1B[38;5;" + colour_num + "m\x1B[48;5;" + colour_num + "m");
	}

	return colour_str;
}
#endif

void set_cursor_position(_cpf_type::stream user_stream, const _cpf_type::str& attrib_str)
{
	auto comma_pos = attrib_str.find(',');
	auto horizontal_pos_str = attrib_str.substr(0, comma_pos);
	auto vertical_pos_str = attrib_str.substr(comma_pos + 1);

#ifdef _WIN32
	auto horizontal_pos = atoi(horizontal_pos_str.c_str());
	auto vertical_pos = atoi(vertical_pos_str.c_str());

	COORD coords;
	coords.X = static_cast<SHORT>(horizontal_pos);
	coords.Y= static_cast<SHORT>(vertical_pos);
	BOOL result = SetConsoleCursorPosition(
		user_stream == stdout ? stdout_handle : stderr_handle, 
		coords);

	if (!result)
	{
		throw _cpf_type::error("cpf err: invalid coordinates");
	}
#else
	//http://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
	fprintf(user_stream, "%s", ("\x1B[" + vertical_pos_str + ";" + horizontal_pos_str+"H").c_str());
#endif
}

void clear_terminal_buffer(	_cpf_type::stream user_stream,
							const _cpf_type::str& attrib)
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
	
	if (attrib == "!")
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
	if(attrib == "!")
	{
		fprintf(user_stream, "\x1B[2J\x1B[H");
	}
	
	if (attrib == "!~")
	{
		fprintf(user_stream, "\x1B[2J");
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

void input_wait(_cpf_type::stream user_stream, const _cpf_type::str& attrib)
{
	if (attrib ==  "|")
	{
		_getch();
	}
	else // |^
	{
		int c;
		while(c = std::getchar() != '\n');
	}
}

#ifdef _WIN32

void config_text_col(_cpf_type::stream user_stream, _cpf_type::colour user_colour, char col_config_type)
{
	if(col_config_type == 0) //foreground
	{
		CONSOLE_SCREEN_BUFFER_INFOEX cbie; //hold info

		//article didn't say this was necessary, but to be on the safe side...
		cbie.cbSize = sizeof (CONSOLE_SCREEN_BUFFER_INFOEX);

		GetConsoleScreenBufferInfoEx (user_stream == stdout ? stdout_handle : stderr_handle, &cbie); //get info

		//first, cancel out all foreground attributes
		//then, set the ones you want (I use bright red)
		cbie.wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cbie.wAttributes |= (FOREGROUND_RED | FOREGROUND_INTENSITY);

		SetConsoleScreenBufferInfoEx (user_stream == stdout ? stdout_handle : stderr_handle, &cbie); //pass updated info back
	}
	else if(col_config_type == 1) //foreground and background
	{

	}
	else if(col_config_type == 2) //background
	{
		SetConsoleTextAttribute(user_stream == stdout ? stdout_handle : stderr_handle, 
								user_colour);
	}
}

#endif

CPF_API void _cpf_config_terminal(	_cpf_type::stream user_stream,
									const _cpf_type::attribs& attribs)
{
	
	if (_cpf_is_fstream(user_stream))
	{
		return; //no configurations necessary if writing to file
	}

	if (_cpf_current_text_attribs != attribs)
	{
		_cpf_current_text_attribs = attribs;

		for (auto a = std::begin(attribs); a != std::end(attribs); ++a)
		{
			auto c_repr = *a;

			if (is_cursor_pos_attrib(c_repr))
			{
				set_cursor_position(user_stream, c_repr);
			}
			else if (c_repr == "|" || c_repr == "|^") /*halt until input*/
			{
				input_wait(user_stream, c_repr);
			}
			else if (c_repr == "!" || c_repr == "!~") /*clear screen*/
			{
				clear_terminal_buffer(user_stream, c_repr);
			}
			else if (c_repr == "?") /*restore colour to system default*/
			{
				restore_terminal_settings(user_stream);
			}
			else /*is_cursor_pos_attrib*/
			{
				auto is_bmct = is_bitmap_colour_token(*a);
#ifdef _WIN32
				if (is_bmct)
				{
					//because windows does not support that.
					continue;
				}

				_cpf_type::colour col = safely_get_terminal_value(c_repr);

				SetConsoleTextAttribute(user_stream == stdout ? stdout_handle : stderr_handle, 
										col);
#else
				_cpf_type::str fstr;

				if(is_bmct)
				{
					fstr = get_terminal_bitmap_colour_value(c_repr);
				}
				else
				{
					fstr = safely_get_terminal_value(c_repr);
				}

				fprintf(user_stream, "%s",	fstr.c_str());
#endif
			}/*is_cursor_pos_attrib*/
		}
	}
}