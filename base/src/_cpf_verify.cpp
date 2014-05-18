#include "_cpf_verify.h"
#include <string>

extern void _cpf_verify(const char* format)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw _cpf_types::error("bad format specifier");
	}
}