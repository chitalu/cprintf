#include <cprintf/internal/cpf_carg.h>

CPF_API void cpf::intern::fmtspec_to_argtype_check(cpf::type::cstr format) {
  for (; *format; ++format) {
    if (*format != '%' || *++format == '%') {
      continue;
    }
    throw CPF_ARG_ERR; // bad format specifier
  }
}
