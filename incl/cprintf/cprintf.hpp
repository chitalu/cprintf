#pragma once

#include <cprintf/internal/cpf_hlpr.h>

template <typename F, typename... Fs>
int cprintf_(FILE* stream, F f, Fs... args)
{
	using namespace cpf::type;
	using namespace cpf::intern;

	static_assert(is_permitted_string_type_<F>::value,
	              "Error: Invalid format string type.");
	verify_args_<Fs...> verify_args;

	int err_code = 0;

	CPF_MARK_CRITICAL_SECTION_;
	{
		err_code = dispatch(stream, std::forward<F>(f), std::forward<Fs>(args)...);
	}
	CPF_UNMARK_CRITICAL_SECTION_;

	return err_code;
}

#include <cprintf/internal/cpf_dbgh.h>

