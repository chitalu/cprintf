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

#ifndef _CPF_PARSE_H
#define _CPF_PARSE_H

#include "c_printf/internal/_cpf_type.h"

/*
	text attribute token escape sequences..
*/
extern const std::initializer_list<_cpf_type::str> attribute_escape_sequences;

/*

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
extern const std::initializer_list<char> std_format_specifiers;

/*
	'd', 'f', 's', 'e', 'o', 'x'
*/
extern const std::initializer_list<char> extended_format_specifier_terminators;

/*
	'+', '-', '.', '*', '#', 'l'
*/
extern const std::initializer_list<char> intermediate_format_specifers;

/*

\a audible alert (bell)
\b backspace
\f form feed
\n newline (linefeed)
\r carriage return
\t tab
\v vertical tab

*/
extern const std::initializer_list<char> escape_characters;

/*
	as indicated by name, this function takes the source 
	format string and which then is subsequently passed through various
	parsing stages. On return, the function yeilds a map of [key] type
	std::size_t and [value] type string-pair. The [key] represents the positional
	offset, within the processed format string, of the second-element-of [value]. 
	The [value] is one in which the first element is a 
	symbolic string token representing the colour the second (format substring)
	element in the pair is to be.
*/
extern _cpf_type::meta_format_type _cpf_process_format_string(
	const _cpf_type::str &fstring);

#endif