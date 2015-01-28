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
			"templated c-string wrapper"

			This is useful for a couple of reasons. Once we have created an
			object of this type we store the size of the string in it: we no
			longer need to use templates. While templates are useful for many
			compile-time applications, using them incurs cost of instantiating
			more and more of them. Second, We can pass our objects by value.
			While passing literals by a reference to array worked in basic example,
			it will not work if we use it in conditional operator, because using
			conditional operator with a throw in it, requires an array-to-pointer
			decay.

			used to simply assert string laterals
		*/
		template<class T = wchar_t>
		class strl_
		{
			const T *  begin_;
			unsigned size_;

		public:
			typedef T UType;

			/*
				a constructor template with different sizes of the array. This is the
				only template that we need. In the constructor we change template parameter
				for class member, and henceforth we can use a normal, non-template class.
				We initialize the pointer with a reference to array. Pointers to objects
				are valid for constexpr functions and constructors, provided they point
				to constexpr objects.
			*/
			template< unsigned N >
			/*constexpr*/ strl_(const T(&arr)[N]) :
				begin_(arr),
				size_(N - 1)
			{
				/*
					Size of string literal is always at least 1 due to the terminating
					zero; hence the assertion, and N - 1 in the initializer.
				*/
				static_assert(N >= 2, "CPF-CT-ERR: expected string-literal with atleast 1 character");
			}

			/*constexpr*/ operator const T *(void) const	{ return begin_; }
			/*constexpr*/ unsigned size(void) const	{ return size_; }
		};

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

		struct except
		{
		private:
			str m_msg;
		public:
			except(void) :m_msg(L"CPF-RT-ERR"){}
			except(const str _msg) :m_msg(_msg){}
			~except(void){}

			inline const wchar_t* wmsg(void)const
			{
				return m_msg.c_str();
			}
			inline const char* msg(void)const{
				return "FIX THIS!"; //cpf::intern::nconv(m_msg);	}
			}

			inline void operator=(str &m)
			{
				m_msg = m;
			}
		};

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
	}
}

#endif 
