#include <cprintf/internal/cpf_carg.h>

namespace _cprintf_
{

	CPF_API void format_specifier_correspondence_check(unicode_character_string_ptr_t format) {
		for (; *format; ++format) {
			if (*format != '%' || *++format == '%') {
				continue;
			}
			throw CPF_ARG_ERR; // bad format specifier
		}
	}
}