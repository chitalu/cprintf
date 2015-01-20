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

#ifndef __CPF_PARSE_H__
#define __CPF_PARSE_H__

#include "cprintf/internal/_cpf_type.h"

namespace cpf
{
	namespace intern
	{
		/*
			text attribute token escape sequences..
		*/
		CPF_API const std::initializer_list<type::str> attr_esc_seqs;

		/*
			standard format specifiers

			%c print a singlecharacter
			%d print a decimal (base 10) number
			%e print an exponential floating-point number
			%f print a floating-point number
			%g print a general-format floating-point number
			%i print an integer in base 10
			%o print a number in octal (base 8)
			%s print a string of characters
			%u print an unsigned decimal (base 10) number
			%x print a number in hex idecimal (base 16)
			%% print a percent sign (\% also works)
		*/
		CPF_API const std::initializer_list<wchar_t> std_fmt_specs;

		/*
			extended format specifier terminators
			'd', 'f', 's', 'e', 'o', 'x'
		*/
		CPF_API const std::initializer_list<wchar_t> ext_fmtspec_terms;

		/*
			intermediate format specifiers
			'+', '-', '.', '*', '#', 'l'
			*/
		CPF_API const std::initializer_list<wchar_t> inter_fmt_specs;

		/*

		\a audible alert (bell)
		\b backspace
		\f form feed
		\n newline (linefeed)
		\r carriage return
		\t tab
		\v vertical tab

		*/
		CPF_API const std::initializer_list<wchar_t> escape_characters;

		/*
			As indicated by name, this function takes the source
			format string which is then subsequently passed through various
			parsing stages. On return, the function yeilds a map of [key] type
			std::size_t and [value] type <string-vector : string> pair. The [key]
			represents the positional offset, within the source format
			string, of the [second]-element-of [value].
			The [value] is one in which the [first] element is a vector of
			symbolic string tokens the [second] element is to be
			formatted with.
			The second element of [value] is a substring of the source format string.
			*/
		CPF_API cpf::type::meta process_format_string(const type::str &format_string);
	}
}

#endif