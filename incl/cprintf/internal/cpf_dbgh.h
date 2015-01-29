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

#ifndef __CPF_DBGH_H__
#define __CPF_DBGH_H__

#if CPF_DBG_CONFIG

namespace cpf
{
	namespace intern
	{
		CPF_API const cpf::type::str dbg_log_fmt_str;
	}
}

#include <cstdarg> //only used by debug-assist functionality

/*
	The following are auxillary macros ideal for debugging purposes.
	All output is streamed to standard error. Users may use these just as
	they would cprintf, cfprintf, cprintf_t, and cfprintf_t. Permissions
	and limitations imposed reflect those of the aforementioned.
	Macro expansion shall occur only in client debug builds and non else.
	As such, building in release mode results in the macros	expanding to [nothing],
	rendering a client call impotent.
*/

#ifdef CPF_WINDOWS_BUILD
#define CPF_SEP_COND (character == '\\' || character == '/');
#else
#define CPF_SEP_COND ( character == '/');
#endif

#define CPF_DBG_LOG_STAMP\
	const cpf::type::str pname__ = CPF_WIDEN_STRING_LITERAL(__FILE__);\
	typedef struct {\
		bool operator()(char character) const{\
			return CPF_SEP_COND;\
		}\
	}sep__;\
	auto fname__ =  cpf::type::str(std::find_if(pname__.rbegin(), pname__.rend(),sep__()).base(), pname__.end());\
	cprintf<CPF_STDE>(cpf::intern::dbg_log_fmt_str, fname__, __TIME__, __DATE__, __FUNCTION__, __LINE__);

#define cprintf_dbg(f, ...) \
	do{\
		CPF_DBG_LOG_STAMP \
		cprintf<CPF_STDE>(f, ##__VA_ARGS__);\
	}while (0)

#define cprintf_t_dbg(f, t) \
	do{\
		CPF_DBG_LOG_STAMP \
		cprintf_t<CPF_STDE>(f, t); \
	} while (0)

#else // do nothing...

#define cprintf_dbg(f, ...) 
#define cprintf_t_dbg(f, t) 

#endif /*#if CPF_DBG_CONFIG*/

#endif