#pragma once

#include <cprintf/internal/cpf_hlpr.h>

template <typename FormatType, typename... ArgumentTypes>
int cprintf_(FILE* stream, FormatType format, ArgumentTypes... arguments)
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


#include <cprintf/internal/cpf_dbgh.h>

