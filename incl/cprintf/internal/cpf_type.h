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

#include <cprintf/internal/cpf_base.h>

#include <inttypes.h>
#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <array>

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
			static_assert(sizeof(T) == 1, "CPF-CT-ERR: invalid template argument size for byte_t");
			union{
				T value;
			}u;
		};

		template<typename T>
		struct byte2_t
		{
			static_assert(sizeof(T) == 2, "CPF-CT-ERR: invalid template argument size for byte2_t");
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
			static_assert(sizeof(T) == 4, "CPF-CT-ERR: invalid template argument size for byte4_t");
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
			static_assert(sizeof(T) == 8, "CPF-CT-ERR: invalid template argument size for byte8_t");
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

		// API return type holds the return code signifying the status
		// of a particular invocation. The return code can be a value 
		// set to any of the possible values defined in cpf_base.h
		// As an auxilliary feature this type also returns the format string
		// specified by the user.
		template<typename T>
		class ret_t : ftype_t<T>
		{
			static_assert(is_string_t<T>::value, "CPF-CT-ERR: invalid format string type");
		public:
			// user format string
			typename std_str_t<typename ftype_t<T>::type>::type f;

			// API return code
			rcode_t c;

			template<typename U>
			inline bool operator==(const typename std::enable_if<std::is_integral<U>::value, U>::type &rhs)
			{
				return (c == rhs);
			}

		};

		template<typename T0 = str, typename ...Ts>
		class uarg_t
		{
			static_assert(is_string_t<T0>::value, "CPF-CT-ERR: invalid uarg_t separator type");
		public:
			typedef typename std_str_t<typename ftype_t<typename std::enable_if<is_string_t<T0>::value, T0>::type>::type>::type cpf_sep_t;

			uarg_t(const cpf_sep_t &seperator) :
				cpf_s_(seperator)
			{		
				static_assert(sizeof...(Ts) > 0, "CPF-CT-ERR: non-specification of printable argument(s)");
			}

			virtual ~uarg_t(void)
			{	}
		protected:
			//how values in "cpf_values" are separated
			cpf_sep_t cpf_s_;
			arg_pack<Ts...> cpf_v_;
		};

		template <typename T = int, typename... Ts>
		struct check_variadic_parameters_ {
			typedef T current_type;

			typedef typename boolean_type_t<
				is_string_t<current_type>::value || 
				std::is_scalar<current_type>::value
			>::type current;

			typedef check_variadic_parameters_<Ts...> Next;

			static_assert(	current::value && Next::current::value, 
							"CPF-CT-ERR: variadic-argument type not allowed");
		};

		template<std::size_t FLAGS>
		struct check_flags_
		{
			static_assert(	(((FLAGS & CPF_STDO) == CPF_STDO) &&((FLAGS & CPF_STDE) != CPF_STDE)) ||
							(((FLAGS & CPF_STDO) != CPF_STDO) &&((FLAGS & CPF_STDE) == CPF_STDE)),
							"CPF-CT-ERR: invalid stream specification");

			static_assert(	(FLAGS ^ CPF_FLAG_MASK_) <= CPF_FLAG_MASK_,
							"CPF-CT-ERR: invalid API flags detected");
		};
	}
}

#endif 
