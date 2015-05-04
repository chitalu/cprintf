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

#include <inttypes.h>
#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <array>

#include <cprintf/internal/cpf_base.h>

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

		template<typename T>
		struct byte_t
		{
			static_assert(sizeof(T) == 1, "CPRINTF COMPILATION ERROR: invalid template argument size for byte_t");
			union{
				T value;
			}u;
		};

		template<typename T>
		struct byte2_t
		{
			static_assert(sizeof(T) == 2, "CPRINTF COMPILATION ERROR: invalid template argument size for byte2_t");
			union{
				struct{
					byte_t<typename std::conditional<std::is_signed<T>::value, std::int8_t, std::uint8_t>::type> lo, hi;
				};
				std::array<typename std::conditional<std::is_signed<T>::value, std::int8_t, std::uint8_t>::type, 2U> a;
				T value;
			}u;
		};

		template<typename T>
		struct byte4_t
		{
			static_assert(sizeof(T) == 4, "CPRINTF COMPILATION ERROR: invalid template argument size for byte4_t");
			union{
				struct{
					byte2_t<typename std::conditional<std::is_signed<T>::value, std::int16_t, std::uint16_t>::type> lo, hi;
				};
				std::array<typename std::conditional<std::is_signed<T>::value, std::int8_t, std::uint8_t>::type, 4U> a;
				float as_float;
				T value;
			}u;
		};

		template<typename T>
		struct byte8_t
		{
			static_assert(sizeof(T) == 8, "CPRINTF COMPILATION ERROR: invalid template argument size for byte8_t");
			union{
				struct{
					byte4_t<typename std::conditional<std::is_signed<T>::value, std::int32_t, std::uint32_t>::type> lo, hi;
				};
				std::array<typename std::conditional<std::is_signed<T>::value, std::int8_t, std::uint8_t>::type, 8U> a;
				double as_double;
				T value;
			}u;
		};

		typedef byte8_t<std::int64_t> signed_bytes_t;
		typedef byte8_t<std::uint64_t> unsigned_bytes_t;

		template<bool B>
		using boolean_type_t = std::conditional<B, std::true_type, std::false_type>;

		//placeholder type
		struct stub_t{};

		//wide character pointer (signed, unsigned, const, and non-const)
		template<typename T>
		struct is_nchar_ptr_t : 
			boolean_type_t<
				std::is_same<char*, T>::value || std::is_same<unsigned char*, T>::value ||
				std::is_same<signed char*, T>::value || std::is_same<const char*, T>::value ||
				std::is_same<const signed char*, T>::value || std::is_same<const unsigned char*, T>::value
			>::type
		{	};

		//wide character pointer (const and non-const)
		template<typename T>
		struct is_wchar_ptr_t :
			boolean_type_t<std::is_same<wchar_t*, T>::value || std::is_same<const wchar_t*, T>::value>::type
		{	};

		//signifies a char pointer type (wide or narrow)
		template<typename T>
		struct is_char_ptr_t :
			boolean_type_t<is_nchar_ptr_t<T>::value || is_wchar_ptr_t<T>::value>::type
		{	};

		//check if T is a narrow character string type
		template<typename T>
		struct is_nstype_t : boolean_type_t<
			std::is_same<T, std::string>::value ||	is_nchar_ptr_t<T>::value
		>::type
		{	};

		//check if T is a wide character string type
		template<typename T>
		struct is_wstype_t : 
			boolean_type_t<std::is_same<T, std::wstring>::value || is_wchar_ptr_t<T>::value>::type
		{	};

		//checks if the user given format string is of a permitted type
		template<typename T = stub_t>
		struct is_string_t : 
			boolean_type_t<is_wstype_t<T>::value || is_nstype_t<T>::value>::type
		{	};

		template<typename T = stub_t>
		struct is_std_str_t :
			boolean_type_t<std::is_same<T, nstr>::value || std::is_same<T, str>::value>::type
		{	};

		// this struct is used to abstract format-string type checks. Also functions 
		// as holder of the "base" string type used to declare a variable that holds
		// the returned format string from the library in addition to the return 
		// code
		template<typename T>
		struct ftype_t : std::enable_if<is_string_t<T>::value, T>
		{	};

		// helper struct used to define the format string representative STL type. 
		// this type can either be std::string or std::wstring
		template<typename T>
		struct std_str_t : std::conditional<is_wstype_t<T>::value, str, nstr>
		{	};

		template<typename T = str, typename ...Ts>
		struct ubase_t
		{
			static_assert(is_string_t<T>::value, "CPRINTF COMPILATION ERROR: invalid string type");
			typedef T cpf_stype;
			
			//first element is always of type std::[w]string!
			std::tuple<	typename std_str_t<T>::type,
						Ts...
			> cpf_arg;
		};

		template<std::size_t FLAGS_>
		struct verify_flags_
		{
			typedef std::integral_constant<std::size_t, FLAGS_> get;

			static_assert(	(((FLAGS_ & CPF_STDO) == CPF_STDO) && ((FLAGS_ & CPF_STDE) != CPF_STDE)) ||
							(((FLAGS_ & CPF_STDO) != CPF_STDO) && ((FLAGS_ & CPF_STDE) == CPF_STDE)),
							"CPRINTF COMPILATION ERROR: invalid stream specification");

			static_assert(	(FLAGS_ ^ CPF_FLAG_MASK_) <= CPF_FLAG_MASK_,
							"CPRINTF COMPILATION ERROR: invalid API flags detected");
		};

		template<typename T = str>
		struct verify_format_
		{
			typedef T type;
			static_assert(is_string_t<T>::value, "CPRINTF COMPILATION ERROR: Invalid format string type");
		};

		template<typename T>
		struct resolve_capture_t
		{
			typedef typename std::conditional<
				is_wstype_t<T>::value,
				str,
				nstr
			>::type type;
		};

		template <typename T = int, typename... Ts>
		struct verify_args_ {
			typedef T current_type;

			typedef typename boolean_type_t<
				is_string_t<current_type>::value ||
				std::is_scalar<current_type>::value
			>::type current;

			typedef verify_args_<Ts...> Next;

			static_assert(current::value && Next::current::value,
				"CPRINTF COMPILATION ERROR: argument type not allowed");
		};

		template<std::size_t FLAGS, typename FMT = cpf::type::str, typename... VARGs>
		struct verify_
		{
			typedef typename verify_flags_<FLAGS> flags;
			// "FMT" represents the format string type as given by user upon 
			// invoking API routine
			typedef typename verify_format_<FMT> fmt;
			verify_args_<VARGs...> decl_stub_;
		};

		// API return type holds the return code signifying the status
		// of a particular invocation. The return code can be a value 
		// set to any of the possible values defined in cpf_base.h
		// As an auxilliary feature this type also returns the format string
		// specified by the user.
		template<typename DETAIL>
		struct status_t : 
			ubase_t<typename std_str_t<typename DETAIL::fmt::type>::type,
					typename std::conditional<	(DETAIL::flags::get::value & CPF_CAPTURE) == CPF_CAPTURE,
												typename resolve_capture_t<typename DETAIL::fmt::type>::type,
												stub_t>::type>
		{
		public:

			// API return code
			rcode_t c;
			
			inline bool operator==(const rcode_t &rhs)
			{
				return (c == rhs);
			}

		};

		template<typename T, typename U, typename ...Us>
		struct uarg_t :
			verify_args_<T, U, Us...>,
			ubase_t<typename std_str_t<T>::type, U, Us...>
		{
			static_assert(is_string_t<T>::value, "CPRINTF COMPILATION ERROR: Invalid separator type");
			typedef typename ubase_t<typename std_str_t<T>::type, U, Us...>::cpf_stype ubase_stype_t;
		public:

			uarg_t(void)
			{	
				//set default value to be a space 
				std::get<0>(this->cpf_arg) = 
				ubase_stype_t(
					1,
					std::conditional<	
						is_wstype_t<T>::value,
						std::integral_constant<wchar_t, ' '>,
						std::integral_constant<char, ' ' >
					> ::type::value
				);
			}

			uarg_t(ubase_stype_t seperator)
			{	std::get<0>(this->cpf_arg) = (seperator);	}

			virtual ~uarg_t(void)
			{	}
		};
	}
}

#endif 
