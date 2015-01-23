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
		/*
			narrow character string debug log
		*/
		CPF_API const cpf::type::nstr dbg_log_fmt_nstr;

		/*
			wide character string debug log
		*/
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

#ifdef _WIN32
#define CPF_SEP_COND (character == '\\' || character == '/');
#else
#define CPF_SEP_COND ( character == '/');
#endif

/*
	Note the the anonymous struct for os specific dir path wrangling
*/
#define CPF_DBG_LOG_WRITE(api_func, log_str_t, log_str)\
	typedef struct {\
		bool operator()(char character) const{\
			return CPF_SEP_COND;\
		}\
	}fpath_sep_func;\
	auto fname =  log_str_t(\
	std::find_if(pathname.rbegin(), pathname.rend(),fpath_sep_func()).base(),\
	pathname.end());\
	api_func(stderr, log_str, \
	fname.c_str(), __TIME__, __DATE__, __FUNCTION__, __LINE__);

/*
	wide character string variants
*/
#define CPF_DBG_LOG_STR\
	const cpf::type::str pathname = cpf::wconv(__FILE__);\
	CPF_DBG_LOG_WRITE(cfwprintf, cpf::type::str, cpf::intern::dbg_log_fmt_str.c_str())

#define cfwprintf_dbg(ustream, format, ...) \
	do{\
	CPF_DBG_LOG_STR \
	cfwprintf(ustream, format, ##__VA_ARGS__);\
	}while (0);

#define cwprintf_dbg(format, ...) \
	cfwprintf_dbg(stderr, format, ##__VA_ARGS__)

#define cfwprintf_t_dbg(ustream, format, tup) \
	do{\
	CPF_DBG_LOG_STR \
	cfwprintf_t(ustream, format, tup); \
	} while (0);\

#define cwprintf_t_dbg(format, tup) \
	cfwprintf_t_dbg(stderr, format, tup);

/*
	narrow character string variants
*/
#define CPF_DBG_LOG_NSTR\
	const cpf::type::nstr pathname = __FILE__;\
	CPF_DBG_LOG_WRITE(cfprintf, cpf::type::nstr, cpf::intern::dbg_log_fmt_nstr.c_str())

#define cfprintf_dbg(ustream, format, ...) \
	do{\
	CPF_DBG_LOG_NSTR \
	cfprintf(ustream, format, ##__VA_ARGS__);\
	}while (0);

#define cprintf_dbg(format, ...) \
	cfprintf_dbg(stderr, format, ##__VA_ARGS__)

#define cfprintf_t_dbg(ustream, format, tup) \
	do{\
	CPF_DBG_LOG_NSTR \
	cfprintf_t(ustream, format, tup); \
	} while (0);\

#define cprintf_t_dbg(format, tup) \
	cfprintf_t_dbg(stderr, format, tup);

#else // do nothing...

#define cfwprintf_dbg(ustream, format, ...) 
#define cwprintf_dbg(format, ...) 
#define cfwprintf_t_dbg(ustream, format, tup) 
#define cwprintf_t_dbg(format, tup) 

#define cfprintf_dbg(ustream, format, ...) 
#define cprintf_dbg(format, ...) 
#define cfprintf_t_dbg(ustream, format, tup) 
#define cprintf_t_dbg(format, tup) 

#endif /*#if CPF_DBG_CONFIG*/

#endif