#ifndef COLOUR_REPR_H
#define COLOUR_REPR_H

#include <map>
#include <string>
#include <vector>

/*S_T_A_UNDEF = system text attribute undefined*/

#ifdef _WIN32
#include <Windows.h>
typedef WORD colour_t;
#define S_T_A_UNDEF 666
#else
#define S_T_A_UNDEF "undef"
typedef std::string colour_t;
#endif

typedef int _cpf_err_t;

#define BLOCK_SPACE_TOKEN "^_6r!|"
/*http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm*/
#define CURSOR_POS_TOKEN "^#10-20|"

extern const std::map<const std::string, colour_t> _cpf_colour_token_vals;
extern const std::vector<std::string> _cpf_colour_tokens;
extern const std::vector<std::string> _cpf_blockspace_tokens;

#endif