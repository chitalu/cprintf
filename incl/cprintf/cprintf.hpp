#pragma once

#include <cprintf/internal/cpf_hlpr.h>

template <typename... Types>
inline int cprintf(decltype(stdout) stream, Types... arguments) noexcept
{
	return _cprintf_::dispatch(stream, std::forward<Types>(arguments)...);
}

template <typename... Types>
inline int cprintf(Types... arguments) noexcept
{
	return cprintf(stdout, std::forward<Types>(arguments)...);
}

#if CPF_DBG_CONFIG
#define cprintf_dbg(format_, ...) cprintf_dbg_impl_(format_, ##__VA_ARGS__);                             
#else
#define cprintf_dbg(format, ...)
#endif