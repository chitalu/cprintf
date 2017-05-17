#ifndef __CPF_FIND_H__
#define __CPF_FIND_H__

#include <cprintf/internal/cpf_base.h>
#include <cprintf/internal/cpf_type.h>

namespace _cprintf_
{

CPF_API std::int32_t search_for(const unicode_string_t &_what,
                                   const unicode_string_t &_where,
                                   const std::int32_t _offset = 0,
                                   const char &_esc_char = CPF_ESC_CHAR);
}
#endif
