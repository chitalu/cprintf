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

#ifndef _CPF_VERIFY_H
#define _CPF_VERIFY_H

#include "_cpf_type.h"
#include "_cpf_type_norm.h"
#include <assert.h>

/*

*/
extern void _cpf_verify(_cpf_type::c_str format);

/*
	verifies that the format string contains arguments which
	match the given % sequence(s) in the correct order.
	note that this is only able to test those format specifiers
	found in "std_format_specifiers"
*/
template<class T, typename... Ts> void
_cpf_verify(_cpf_type::c_str format, const T& farg, const Ts&&... args)
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
			break;
		case 'e':
			assert(std::is_floating_point<T>::value);
			break;
		case 'g':
			assert(std::is_floating_point<T>::value);
			break;
		case 'd':
			assert(std::is_integral<T>::value);
			break;
		case 'i':
			assert(std::is_integral<T>::value);
			break;
		case 'o':
			assert(std::is_integral<T>::value);
			break;
		case 'u':
			assert(std::is_integral<T>::value);
			break;
		case 'c':
			assert(std::is_integral<T>::value);
			break;
		case 'x':
			assert(std::is_integral<T>::value);
			break;
		case 'l': //note that this is actually in "intermediate_format_specifers"
			assert(std::is_integral<T>::value);
			break;
		case '#': //note that this is actually in "intermediate_format_specifers"
			assert(std::is_integral<T>::value);
			break;
		case 's':
			assert(std::is_pointer<T>::value);
			break;
		default:
			fprintf(stderr, "warning: unsupported formatter: %c\n", f);
			break;
		}
		return _cpf_verify(++format, std::forward<Ts>(args)...);
	}

	throw _cpf_type::error("format specifier (%) count does not match argument count");
}

#endif /*#ifndef _CPF_VERIFY_H*/