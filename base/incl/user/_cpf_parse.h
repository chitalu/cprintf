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

#include "_cpf_type.h"

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