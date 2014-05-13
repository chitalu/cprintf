#ifndef _CPF_PARSE_H
#define _CPF_PARSE_H

#include "common.h"
#include "type_norm.h"

/*

*/
extern _cpf_types::_string_type_ _cpf_block_space_token_parse(
	const _cpf_types::_string_type_ &src_format);

/*
	
*/
extern _cpf_types::meta_format_type _cpf_colour_token_parse(
	const _cpf_types::_string_type_ &formatter);

extern _cpf_types::_string_type_ _cpf_tag_map_token_parse(
	const _cpf_types::_string_type_ &formatter);

/*

*/
extern void _cpf_authenticate_format_string(const char* format);

/*

*/
template<class T, typename... Ts> void
_cpf_authenticate_format_string(const char* format, const T& farg, const Ts&&... args)
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

	throw std::invalid_argument("Too few format specifiers.");
}

#endif