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

namespace cpf
{
	namespace type
	{

		/*library native string type...*/
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
			const str m_msg;
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
