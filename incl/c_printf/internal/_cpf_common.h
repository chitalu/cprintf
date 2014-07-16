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

/*
interesting:
http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
http://bjh21.me.uk/all-escapes/all-escapes.txt
*/

#ifndef _CPF_COMMON_H
#define _CPF_COMMON_H

#ifdef _WIN32 /* windows */

/*
 * CPF_BUILD_AS_STATIC is defined for static library.
 * CPF_FUNC_EXPORT  is defined for building the DLL library.
 */

#ifdef CPF_BUILD_AS_STATIC
#  define CPF_API extern
#else
#  ifdef CPF_FUNC_EXPORT
#    define CPF_API extern __declspec(dllexport)
#  else
#    define CPF_API extern __declspec(dllimport)
#  endif
#endif

#define SYSTXTATTRIB_UNDEF (0xFF)
 
#else /* UNIX */

#ifdef CPF_BUILD_AS_STATIC
#  define CPF_API extern
#else
#  if defined(__GNUC__) && __GNUC__>=4
#   define CPF_API extern __attribute__ ((visibility("default")))
#  else
#   define CPF_API extern
#  endif
#endif

#define SYSTXTATTRIB_UNDEF ("undef")
#endif

#include "_cpf_type.h"

#include <stdexcept>
#include <algorithm>

#endif //_CPF_COMMON_H