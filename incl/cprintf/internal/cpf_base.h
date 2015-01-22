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

#ifndef __CPF_BASE_H__
#define __CPF_BASE_H__

#if !defined(CPF_DBG_CONFIG) /*... in compiler flags*/

#ifndef NDEBUG
#	define CPF_DBG_CONFIG 1
#else
#	define CPF_DBG_CONFIG 0
#endif /* NDEBUG */

#endif /* !defined(CPF_DBG_CONFIG) */

#ifdef _WIN32 /* windows */

/*
 * CPF_BUILD_AS_SHARED is defined for shared library.
 * CPF_FUNC_EXPORT  is defined for building the DLL library.
 */

#ifndef CPF_BUILD_AS_SHARED
#  define CPF_API extern
#else
#  ifdef CPF_FUNC_EXPORT
#    define CPF_API extern __declspec(dllexport)
#  else
#    define CPF_API extern __declspec(dllimport)
#  endif
#endif
 
#else /* UNIX */

#ifndef CPF_BUILD_AS_SHARED
#  	define CPF_API extern
#else
#  	ifdef CPF_FUNC_EXPORT
#  		if defined(__GNUC__) && __GNUC__>=4
#   		define CPF_API extern __attribute__ ((visibility("default")))
#		else
#   		#pragma warning Unknown dynamic link import/export semantics.
#  		endif
#  	else
#		if defined(__GNUC__) && __GNUC__>=4
#   		define CPF_API 
#		else
#   		#pragma warning Unknown dynamic link import/export semantics.
#  		endif
#  	endif
#endif

#endif

#include "cpf_type.h"

#include <stdexcept>
#include <algorithm>

#define CPF_ESC_CHAR ('`')

/*
	Note:	preprocessor defintions contained hereinafter
			this "#include" may conflict with user code.
*/
#include <ciso646>

#endif //__CPF_COMMON_H__