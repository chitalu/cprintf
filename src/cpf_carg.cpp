#include <cprintf/internal/cpf_carg.h>

namespace _cprintf_
{

	CPF_API void _cprintf_::format_specifier_correspondence_check(_cprintf_::unicode_character_string_ptr_t format) {
		for (; *format; ++format) {
			if (*format != '%' || *++format == '%') {
				continue;
			}
			throw CPF_ARG_ERR; // bad format specifier
		}
	}
}