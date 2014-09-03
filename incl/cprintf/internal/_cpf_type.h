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

#ifndef _CPF_TYPES_H
#define _CPF_TYPES_H

#ifdef _WIN32
#include <Windows.h>
#endif

#include "cprintf/internal/_cpf_common.h"

#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>

#define _CPF_DISABLE 0x00
#define _CPF_ENABLE 0xFF

namespace cpf
{
	namespace type
	{
		typedef std::wstring wstr;
		typedef std::string nstr;
		typedef std::wstring str;//to go

		/*the library's native string element type -> wchar_t*/
		typedef cpf::type::wstr::allocator_type::value_type wide_element;
		typedef cpf::type::nstr::allocator_type::value_type narrow_element;

		/*wide character string pointer type -> "const wchar_t*" */
		typedef const cpf::type::wide_element* wide_elem_ptr;
		typedef const cpf::type::wide_element* cstr;//to go

		/*narrow character string pointer type -> "const char"*/
		typedef const cpf::type::narrow_element* narrow_elem_ptr;

		typedef cpf::type::wstr::size_type size;//to go

		typedef cpf::type::wstr::size_type wsize;
		typedef cpf::type::nstr::size_type nsize;

#ifdef _WIN32
		typedef WORD colour;
#else
		typedef cpf::type::wstr colour;
#endif

		struct except
		{
		private:
			cpf::type::wide_elem_ptr m_msg;
		public:
			except(void) :m_msg(L"cpf err"){}
			except(const cpf::type::wide_elem_ptr _msg) :m_msg(_msg){}
			~except(void){}

			inline cpf::type::wide_elem_ptr wmsg(void)
			{ return m_msg; }
			inline cpf::type::narrow_elem_ptr msg(void)
			{ return reinterpret_cast<cpf::type::narrow_elem_ptr>(m_msg); /*fix this*/}
		};

		typedef std::pair<cpf::type::wstr, cpf::type::wstr> string_pair;
		typedef std::vector<cpf::type::wstr> string_vector;
		typedef string_vector attribute_group;
		typedef std::map<cpf::type::size, std::pair<string_vector, cpf::type::wstr>> meta;
		typedef cpf::type::meta::iterator meta_iterator;
		typedef cpf::type::meta::const_iterator c_meta_iterator;
		typedef std::map<const cpf::type::wstr, cpf::type::colour> token_value_map;

		template<typename ...Ts>
		using arg_pack = std::tuple<Ts...>;

		typedef FILE* stream;

#ifndef NDEBUG
		/*	os specific dir path wrangling	*/
		struct fpath_sep_func
		{
			bool operator()(char character) const
			{
#ifdef _WIN32
				return character == '\\' || character == '/';
#else
				return character == '/';
#endif
			}
		};
#endif
	}
}

#endif 