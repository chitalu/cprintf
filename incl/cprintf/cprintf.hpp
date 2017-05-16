#pragma once

#include <cprintf/internal/cpf_hlpr.h>

template <typename FormatType, typename... ArgumentTypes>
int cprintf(cpf::type::stream_t stream,
            FormatType          format,
            ArgumentTypes... arguments)
{
	using namespace cpf::type;
	using namespace cpf::intern;

	static_assert(is_permitted_string_type_<FormatType>::value,
	              "Error: Invalid format string type.");
	verify_args_<ArgumentTypes...> verify_args;

	std::lock_guard<std::mutex> lock(cpf::intern::user_thread_mutex);

	int error_code = dispatch(stream, std::forward<FormatType>(format),
	                          std::forward<ArgumentTypes>(arguments)...);

	return error_code;
}

template <typename FormatType, typename... ArgumentTypes>
int cprintf(FormatType format, ArgumentTypes... arguments)
{
	return cprintf(stdout, std::forward<FormatType>(format),
	               std::forward<ArgumentTypes>(arguments)...);
}

#if CPF_DBG_CONFIG

#define cprintf_dbg(format_, ...)                                        \
	do                                                                     \
	{                                                                      \
		const cpf::type::str_t pname__ = CPF_WIDEN_STRING_LITERAL(__FILE__); \
		typedef struct                                                       \
		{                                                                    \
			bool operator()(char character) const                              \
			{                                                                  \
				return CPF_FILE_PATH_SEPARATOR                                   \
			}                                                                  \
		} sep__;                                                             \
		auto fname__ = cpf::type::str_t(                                     \
		  std::find_if(pname__.rbegin(), pname__.rend(), sep__()).base(),    \
		  pname__.end());                                                    \
		cprintf_(stderr, cpf::intern::dbg_log_fmt_str, fname__, __TIME__,    \
		         __DATE__, __FUNCTION__, __LINE__);                          \
		cprintf(stderr, format_, ##__VA_ARGS__);                             \
	} while (false)

#else
#define cprintf_dbg(format, ...)
#endif

