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

#ifndef __CPF_TYPE_H__
#define __CPF_TYPE_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include "cprintf/internal/cpf_base.h"

#include <inttypes.h>
#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>

namespace cpf
{
	namespace type
	{
		/*
		library native string types...
		*/
		typedef std::wstring str;
		typedef std::string nstr;

		typedef const wchar_t* cstr;

		typedef cpf::type::str::size_type size;
		typedef cpf::type::nstr::size_type nsize;

#ifdef _WIN32
		typedef WORD colour;
#else
		typedef cpf::type::str colour;
#endif

		typedef std::pair<cpf::type::str, cpf::type::str> string_pair;
		typedef std::vector<cpf::type::str> string_vector;
		typedef string_vector attribute_group;
		typedef std::map<cpf::type::size, std::pair<string_vector, cpf::type::str>> meta;
		typedef cpf::type::meta::iterator meta_iterator;
		typedef cpf::type::meta::const_iterator c_meta_iterator;
		typedef std::map<const cpf::type::str, cpf::type::colour> token_value_map;

		template<typename ...Ts>
		using arg_pack = std::tuple<Ts...>;

		typedef FILE* stream;

		typedef int rcode_t;

		//placeholder type
		struct stub_t {};

		//check if T is a narrow character string type
		template<typename T>
		struct is_nstype_t : 
			std::conditional<
				std::is_same<T, std::string>::value || 
				std::is_pointer<T>::value &&
				(
					std::is_same<char*, T>::value || std::is_same<unsigned char*, T>::value || 
					std::is_same<signed char*, T>::value || std::is_same<const char*, T>::value ||	
					std::is_same<const signed char*, T>::value || std::is_same<const unsigned char*, T>::value
				),
				T, //first type
				stub_t//second type
			>
		{	};

		//check if T is a wide character string type
		template<typename T>
		struct is_wstype_t : 
			std::conditional<
				std::is_same<T, std::wstring>::value ||
				std::is_pointer<T>::value && (std::is_same<wchar_t*, T>::value || std::is_same<const wchar_t*, T>::value),
				T, //first type
				stub_t //second type
			>
		{	};

		// this struct is used to abstract format-string type checks. Also functions 
		// as holder of the "base" string type used to declare a variable that holds
		// the returned format string from the library in addition to the return 
		// code
		template<typename T>
		struct ftype_t : 
			public std::enable_if <
				!std::is_same<typename is_wstype_t<T>::type, stub_t>::value || 
				!std::is_same<typename is_nstype_t<T>::type, stub_t>::value, 
				T 
			>
		{};

		// helper struct used to define the format string representative STL type. 
		// this type can either be std::string or std::wstring
		template<typename T>
		struct fstr_t : 
			std::conditional<	!std::is_same<typename is_wstype_t<T>::type, stub_t>::value,
								str, 
								nstr
							>
		{	};

		// API return type holds the return code signifying the status
		// of a particular invocation. The return code can be a value 
		// set to any of the possible values defined in cpf_base.h
		// As an auxilliary feature this type also returns the format string
		// specified by the user.
		template<typename T>
		class ret_t : ftype_t<T>
		{
		public:
			// user format string
			typename fstr_t<typename ftype_t<T>::type>::type f;

			// API return code
			rcode_t c;

			template<typename U>
			inline bool operator==(const typename std::enable_if<std::is_integral<U>::value, U>::type &rhs)
			{
				return (c == rhs);
			}

		};
	}
}

#endif 
