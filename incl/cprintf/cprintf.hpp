#pragma once

#include <cprintf/internal/cpf_hlpr.h>

#if CPF_DBG_CONFIG
#define cprintf_dbg(format_, ...) cprintf_dbg_impl_(format_, ##__VA_ARGS__);                             
#else
#define cprintf_dbg(format, ...)
#endif

template <typename FormatType, typename... ArgumentTypes>
inline int cprintf(decltype(stdout) file_stream,
            FormatType          format,
            ArgumentTypes... arguments)
{
	using namespace _cprintf_;
	static_assert(is_valid_string_type_<FormatType>::value,
	              "Error: Invalid format string type.");
	verify_args_<ArgumentTypes...> verify_args;

	const int error_code = dispatch(file_stream, std::forward<FormatType>(format),
	                          std::forward<ArgumentTypes>(arguments)...);

	return error_code;
}

template <typename FormatType, typename... ArgumentTypes>
inline int cprintf(FormatType format, ArgumentTypes... arguments)
{
	return cprintf(stdout, std::forward<FormatType>(format),
	               std::forward<ArgumentTypes>(arguments)...);
}

