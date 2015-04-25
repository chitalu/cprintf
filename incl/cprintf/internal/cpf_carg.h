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

#ifndef __CPF_VERIFY_H__
#define __CPF_VERIFY_H__

#include "cprintf/internal/cpf_type.h"
#include <cassert>

namespace cpf
{
	namespace intern
	{
		/*
			format specifier-to-argument correspondence check
			i.e "%d" must correspond to an integral, "%p" to a pointer etc.

			verifies that the format string contains arguments which
			match the given % sequence(s) in the correct order.
			note that this is only able to test those format specifiers
			found in "cpf::intern::std_fmt_specs".

			We perform runtime checks on the validity of arguments when compared to
			their corresonding format specifiers.

			This is only done in client debug builds.
		*/
		CPF_API void fmtspec_to_argtype_check(cpf::type::cstr format);

		template<class T = int, typename... Ts>
		void fmtspec_to_argtype_check(cpf::type::cstr format, T&& farg, Ts&&... args)
		{
			using namespace cpf::type;
			cpf::type::verify_args_<T, Ts...> _;

			for (; *format; ++format)
			{
				if (*format != '%' || *++format == '%')
					continue;

				wchar_t f = *format;

				switch (f)
				{
				case 'f': case 'e': case 'g':
					if (!std::is_floating_point<T>::value)
						throw CPF_ARG_ERR; // expected a[floating point] value
					break;
				case 'd': case 'i': case 'o': case 'u': case 'c': case 'x':
				case 'l': //note that this is actually in "inter_fmt_specs"
				case '#': //note that this is actually in "inter_fmt_specs"
					if (!std::is_integral<T>::value)
						throw CPF_ARG_ERR; // expected an[integral] value"
					break;
				case 's':
					if (!cpf::type::is_string_t<T>::value)
						throw CPF_ARG_ERR; // expected a value of type[c - string, std::string or std::wstring]
					break;
				case 'p':
					if (!std::is_pointer<T>::value)
						throw CPF_ARG_ERR; // expected a[pointer] value
					break;
				case 'b':
					if (!std::is_scalar<T>::value)
						throw CPF_ARG_ERR; // expected a scalar, i.e 
					break;
				default:
					//	Note: does note cover all edge cases
					break;
				}

				return fmtspec_to_argtype_check(++format, std::forward<Ts>(args)...);
			}

			throw CPF_ARG_ERR; // invalid argument count
		}
	}
}

#endif /*#ifndef __CPF_VERIFY_H__*/