#ifndef _CPF_TYPES_H
#define _CPF_TYPES_H

#ifdef _WIN32
#include <Windows.h>
#endif

#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>

#define _CPF_DISABLE 0x00
#define _CPF_ENABLE 0xFF

/*
	flag to specify whether colour configuration is enabled or not.
	by default this value is set to [enabled].
	if disabled, format strings containing c_printf approved text formatting
	tokens will remain unmodified and subsequently displayed as is. No 
	exception will be thrown unless otherwise applicable.
*/
extern std::uint8_t _cpf_colour_config;

/*
	flag to specify whether to automatically include a carridge-return
	at the end of the format string or not. Default behaviour is 
	to include.
*/
extern std::uint8_t _cpf_newline_config;

struct _cpf_err{
private:
	const char* msg;
public:
	_cpf_err(void):msg("_cpf_err"){}
	_cpf_err(const char* _msg):msg(_msg){}
	~_cpf_err(void){}

	inline const char* what(void){return msg;}
};

namespace _cpf_types
{
	typedef std::string _string_type_;

#ifdef _WIN32
	typedef WORD colour;
#else
	typedef std::string colour;
#endif
}

namespace _cpf_types
{
	typedef _cpf_err error;
	typedef std::pair<_cpf_types::_string_type_, _cpf_types::_string_type_> str_pair;
	typedef std::vector<_cpf_types::_string_type_> string_vector;
	typedef string_vector attributes;
	typedef std::map<std::size_t, std::pair<string_vector, _cpf_types::_string_type_>> meta_format_type;
	typedef std::map<const _cpf_types::_string_type_, _cpf_types::colour> colour_token_map;
	
	typedef FILE* stream;
}

#endif 