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

#ifndef _CPF_TYPE_NORM_H
#define _CPF_TYPE_NORM_H

#include "cprintf/internal/_cpf_type.h"
#include <type_traits>
#include <ciso646>

namespace cpf
{
	/*class object envoked by inititialsation on condition that
	a user species an argument that is not supported i.e 
	cannot be printed.*/
	/*class illegal_cprintf_argument_type_{	
	private:
		illegal_cprintf_argument_type_(void)
		{	throw cpf::type::except(L"bad argument!");	};
		~illegal_cprintf_argument_type_(void){};
	};

	template<bool B, class T>
	struct enable_if_ {};

	template<class T>
	struct enable_if_<true, T> { typedef T type; };

	template<class T>
	struct enable_if_<false, T> { typedef illegal_cprintf_argument_type_ type; };

	template<class ARGTYPE>
	typename cpf::enable_if_<(std::is_same<cpf::type::str, ARGTYPE>::value or std::is_same<cpf::type::nstr, ARGTYPE>::value) or
							std::is_pointer<ARGTYPE>::value or
							std::is_floating_point<ARGTYPE>::value or
							std::is_integral<ARGTYPE>::value,
							ARGTYPE>::type
		normalize_arg(const ARGTYPE& arg)
	{
		return arg;
	}*/
}

#endif