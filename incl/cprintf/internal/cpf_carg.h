#ifndef __CPF_VERIFY_H__
#define __CPF_VERIFY_H__

#include "cprintf/internal/cpf_type.h"
#include <cassert>

namespace _cprintf_
{

/*
        format specifier-to-argument correspondence check
        i.e "%d" must correspond to an integral, "%p" to a pointer etc.

        verifies that the format string contains arguments which
        match the given % sequence(s) in the correct order.
        note that this is only able to test those format specifiers
        found in "std_fmt_specs".

        We perform runtime checks on the validity of arguments when compared to
        their corresonding format specifiers.

        This is only done in client debug builds.
*/
CPF_API void format_specifier_correspondence_check(unicode_character_string_ptr_t format);

template <class T = int, typename... Ts>
void format_specifier_correspondence_check(unicode_character_string_ptr_t format, T&& farg, Ts&&... args)
{
	for (; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
			continue;

		wchar_t f = *format;

		switch (f)
		{
			case 'f':
			case 'e':
			case 'g':
			{
				if (!std::is_floating_point<T>::value)
				{
					throw CPF_ARG_ERR; // expected a[floating point] value
				}
			}
			break;
			case 'd':
			case 'i':
			case 'o':
			case 'u':
			case 'c':
			case 'x':
			case 'l': // note that this is actually in "inter_fmt_specs"
			case '#': // note that this is actually in "inter_fmt_specs"
			{
				if (!std::is_integral<T>::value)
					throw CPF_ARG_ERR; // expected an[integral] value"
			}
		}
		break;
		case 's':
		case 'S':
		{
			if (!is_valid_string_type_<T>::value)
			{
				throw CPF_ARG_ERR; // expected a value of type[c - string, std::string
				                   // or std::wstring]
			}
		}
		break;
		case 'p':
		{
			if (!std::is_pointer<T>::value)
			{
				throw CPF_ARG_ERR; // expected a[pointer] value
			}
		}
		break;
		default:
			//	Note: we do not cover all edge cases
			break;
	}

	return format_specifier_correspondence_check(++format, std::forward<Ts>(args)...);
}

}


#endif /*#ifndef __CPF_VERIFY_H__*/
