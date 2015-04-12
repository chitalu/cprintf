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

#if defined(_WIN64) || defined(_WIN32)
#define CPF_WINDOWS_BUILD 1
#elif defined(__APPLE__)
#define CPF_MAC_BUILD 1
#elif defined(__gnu_linux__)
#define CPF_LINUX_BUILD 1
#endif

#if !defined(CPF_DBG_CONFIG) /*... in compiler flags*/

#ifndef NDEBUG
#	define CPF_DBG_CONFIG 1
#else
#	define CPF_DBG_CONFIG 0
#endif /* NDEBUG */

#endif /* !defined(CPF_DBG_CONFIG) */

#ifdef CPF_WINDOWS_BUILD

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
 
#elif defined(CPF_LINUX_BUILD) /*| defined(CPF_MAC_BUILD)*/

#ifndef CPF_BUILD_AS_SHARED
#  	define CPF_API extern
#else
#  	ifdef CPF_FUNC_EXPORT
#  		if defined(__GNUC__) && __GNUC__>=4
#   		define CPF_API extern __attribute__ ((visibility("default")))
#		else
#   		pragma warning Unknown dynamic link import/export semantics.
#  		endif
#  	else
#		if defined(__GNUC__) && __GNUC__>=4
#   		define CPF_API 
#		else
#   		pragma warning Unknown dynamic link import/export semantics.
#  		endif
#  	endif
#endif

#endif /*#ifdef CPF_WINDOWS_BUILD*/

#include "cpf_type.h"

#include <stdexcept>
#include <algorithm>

#define CPF_ESC_CHAR ('`')

// from narrow to wide e.g. const char* -> const wchar_t*
// string literals only.
#define CPF_WIDEN_2__(x) L##x
#define CPF_WIDEN_1__(x) CPF_WIDEN_2__(x)
#define CPF_WIDEN_STRING_LITERAL(s) CPF_WIDEN_1__(s)

/*
	API flags

	cprintf<FLAGS>(...);
*/

// write to standard output stream (default)
#define CPF_STDO 0x1

// write to standard error stream
#define CPF_STDE 0x2

// Flag to guarrantee atomicity of API invocation such that no other client thread shall 
// execute instructions within until the [current client thread of execution] has finished.
// Should the user fail to specify this value as a template parameter to the API in their
// mutlithreaded program, the behavior and outcome are undefined.
// non-specification implies a sequential client program. Note however that no part of the API 
// is re-entant, and as such it is the users' responsibilty to enable atomicity via
// CPF_ATOMIC if using multiple threads in a client program.
#define CPF_ATOMIC 0x4

#define CPF_FLAG_MASK_ (CPF_STDO | CPF_STDE | CPF_ATOMIC)

/*
	error codes.
*/

// no error (every is just fine)
#define CPF_NO_ERR 0x0000

// invalid token(s) encountered in format string
#define CPF_TOKEN_ERR 0xC001

// invalid format specifier(s) in format string
#define CPF_FSPEC_ERR 0xC002

// mismatch between a format specifier and its corresponding argument 
#define CPF_ARG_ERR 0xC003

// error occurred following internal system configurations 
#define CPF_SYSTEM_ERR 0xC004

// error occurred while attempting to convert narrow-character string
// to its wide-character form for internal use. 
#define CPF_NWCONV_ERR 0xC005

/*
	Note:	preprocessor defintions contained hereinafter
			this "#include" may conflict with user code.
*/
#include <ciso646>

#endif //__CPF_COMMON_H__
