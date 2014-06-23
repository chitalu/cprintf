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

#include "c_printf/internal/_cpf_type.h"
#include <type_traits>

template<class T>
typename std::enable_if<std::is_integral<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

template<class T>
typename std::enable_if<std::is_pointer<T>::value, T>::type
normalize_arg(const T arg)
{
	return arg;
}

extern _cpf_type::c_str normalize_arg(const _cpf_type::str& arg);

//extern _cpf_type::c_str normalize_arg(const _cpf_type::ocl_e& arg);

#endif