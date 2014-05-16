#ifndef _CPF_TYPES_H
#define _CPF_TYPES_H

#include <utility>
#include <vector>
#include <map>
#include <string>

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
	typedef std::map<std::size_t, str_pair> meta_format_type;
	typedef std::map<const _cpf_types::_string_type_, _cpf_types::colour> colour_token_map;
	typedef std::vector<_cpf_types::_string_type_> string_vector;
	typedef FILE* stream;
}

#endif 