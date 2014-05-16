#ifndef _CPF_PARSE_H
#define _CPF_PARSE_H

#include "_cpf_types.h"

/*
	as indicated by name, this function takes the source 
	format string and which then is subsequently passed through various
	parsing stages. On return, the function yeilds a map of [key] type
	std::size_t and [value] type string-pair. The [key] represents the positional
	offset, within the processed format string, of the second-element-of [value]. 
	The [value] is one in which the first element is a 
	symbolic string token representing the colour the second (format substring)
	element in the pair is to be.
*/
extern _cpf_types::meta_format_type _cpf_process_format_string(
	const _cpf_types::_string_type_ &fstring);

#endif