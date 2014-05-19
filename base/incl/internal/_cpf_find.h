#ifndef CPF_FIND_H
#define CPF_FIND_H

#include "_cpf_common.h"

extern std::size_t _cpf_find(	const _cpf_types::_string_type_& _what, 
								const _cpf_types::_string_type_& _where,
								const std::size_t _offset=0,
								const char& _esc_char='?');

#endif