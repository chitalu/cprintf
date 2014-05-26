#ifndef CPF_FIND_H
#define CPF_FIND_H

#include "_cpf_common.h"

/*
	optimizer. 
	they simply prevent execution of code i.e sanitizing
	output strings by removing escape sequences, when it 
	is not necessary.
*/
extern bool encountered_esc_seq_on_parse;

extern std::size_t _cpf_find(	const _cpf_type::str& _what, 
								const _cpf_type::str& _where,
								const std::size_t _offset=0,
								const char& _esc_char='`');

#endif