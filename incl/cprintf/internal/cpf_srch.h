#ifndef __CPF_FIND_H__
#define __CPF_FIND_H__

#include <cprintf/internal/cpf_base.h>
#include <cprintf/internal/cpf_type.h>

namespace cpf {
namespace intern {
CPF_API cpf::type::size search_for(const cpf::type::str_t &_what,
                                   const cpf::type::str_t &_where,
                                   const cpf::type::size _offset = 0,
                                   const char &_esc_char = CPF_ESC_CHAR);
}
}
#endif
