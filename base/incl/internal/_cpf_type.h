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

namespace _cpf_type
{
	typedef std::string str;
	typedef const char* c_str;
}

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
	_cpf_type::c_str msg;
public:
	_cpf_err(void):msg("_cpf_err"){}
	_cpf_err(const char* _msg):msg(_msg){}
	~_cpf_err(void){}

	inline _cpf_type::c_str what(void){ return msg; }
};

namespace _cpf_type
{
#ifdef _WIN32
	typedef WORD colour;
#else
	typedef _cpf_type::str colour;
#endif
}

namespace _cpf_type
{
	typedef _cpf_err error;
	typedef std::pair<_cpf_type::str, _cpf_type::str> str_pair;
	typedef std::vector<_cpf_type::str> str_vec;
	typedef str_vec attribs;
	typedef std::map<std::size_t, std::pair<str_vec, _cpf_type::str>> meta_format_type;
	typedef std::map<const _cpf_type::str, _cpf_type::colour> colour_token_map;
	
	typedef FILE* stream;
}

#endif 