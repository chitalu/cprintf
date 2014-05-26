#include "_cpf_verify.h"
#include <string>

extern void _cpf_verify(_cpf_type::c_str format)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw _cpf_type::error("bad format specifier");
	}
}