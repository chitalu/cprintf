#ifndef __CPF_DBGH_H__
#define __CPF_DBGH_H__

#if CPF_DBG_CONFIG

namespace cpf
{
namespace intern
{
CPF_API const cpf::type::str_t dbg_log_fmt_str;
}
}

#include <cstdarg> //only used by debug-assist functionality

#ifdef CPF_WINDOWS_BUILD
#define CPF_SEP_COND (character == '\\' || character == '/');
#else
#define CPF_SEP_COND (character == '/');
#endif

#define CPF_DBG_LOG_STAMP                                                     \
	const cpf::type::str_t pname__ = CPF_WIDEN_STRING_LITERAL(__FILE__);        \
	typedef struct                                                              \
	{                                                                           \
		bool operator()(char character) const                                     \
		{                                                                         \
			return CPF_SEP_COND;                                                    \
		}                                                                         \
	} sep__;                                                                    \
	auto fname__ = cpf::type::str_t(                                            \
	  std::find_if(pname__.rbegin(), pname__.rend(), sep__()).base(),           \
	  pname__.end());                                                           \
	cprintf_(stderr, cpf::intern::dbg_log_fmt_str, fname__, __TIME__, __DATE__, \
	         __FUNCTION__, __LINE__);

#define cprintf_dbg(f, ...)                               \
	do                                                      \
	{                                                       \
		CPF_DBG_LOG_STAMP cprintf_(stderr, f, ##__VA_ARGS__); \
	} while (0)

#else // do nothing...

#define cprintf_dbg(f, ...)

#endif /*#if CPF_DBG_CONFIG*/

#endif
