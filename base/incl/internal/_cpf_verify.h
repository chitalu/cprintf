#ifndef _CPF_VERIFY_H
#define _CPF_VERIFY_H

#include "_cpf_types.h"
#include "_cpf_type_norm.h"
#include <assert.h>

/*

*/
extern void _cpf_verify(const char* format);

/*

*/
template<class T, typename... Ts> void
_cpf_verify(const char* format, const T& farg, const Ts&&... args)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		char f = *format;
		switch (f)
		{
		case 'f':
			assert(std::is_floating_point<T>::value);
			break;
		case 'd':
			assert(std::is_integral<T>::value);
			break;
		case 'c':
			assert(std::is_integral<T>::value);
			break;
		case 's':
			assert(std::is_pointer<T>::value);
			break;
		default:
			printf("warning: unsupported formatter: %c\n", f);
			break;
		}
		return check_printf(++format, args...);
	}

	throw _cpf_types::error("Too few format specifiers.");
}

#endif /*#ifndef _CPF_VERIFY_H*/