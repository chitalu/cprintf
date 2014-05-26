#ifndef _CPF_SYS_COLOUR_CONFIG_H
#define _CPF_SYS_COLOUR_CONFIG_H

#include "_cpf_common.h"

#ifdef _WIN32

/*
	auxillary symbols to help shorten code in cpp
	and aid in readability
*/

/*
	*\foreground
*/
#define _cpf_Rf 	(FOREGROUND_RED)
#define _cpf_Gf 	(FOREGROUND_GREEN)
#define _cpf_Bf 	(FOREGROUND_BLUE)
#define _cpf_Yf		(_cpf_Rf | _cpf_Gf)
#define _cpf_Mf		(_cpf_Rf | _cpf_Bf)
#define _cpf_Cf		(_cpf_Gf | _cpf_Bf)
#define _cpf_Wf		(_cpf_Rf | _cpf_Gf | _cpf_Bf)

/*
	*\background
*/
#define _cpf_Rb 	(BACKGROUND_RED)
#define _cpf_Gb 	(BACKGROUND_GREEN)
#define _cpf_Bb 	(BACKGROUND_BLUE)
#define _cpf_Yb		(_cpf_Rb | _cpf_Gb)
#define _cpf_Mb		(_cpf_Rb | _cpf_Bb)
#define _cpf_Cb		(_cpf_Gb | _cpf_Bb)
#define _cpf_Wb		(_cpf_Rb | _cpf_Gb | _cpf_Bb)

/*
	*\intensifiers
*/
#define _cpf_fgi 	(FOREGROUND_INTENSITY)
#define _cpf_bgi	(BACKGROUND_INTENSITY)

#endif /*	#ifdef _WIN32	*/

extern "C" _cpf_type::attribs _cpf_current_text_attribs;
extern "C" const _cpf_type::str_vec _cpf_std_tokens;

/*
	tokens available on both windows and linux
*/
extern const _cpf_type::colour_token_map _cpf_std_token_vals;

/*
	configure system terminal settings

	@strm 	- output stream
	@attr - text attribute vector
*/
extern "C" void _cpf_config_terminal(	_cpf_type::stream strm,
										const _cpf_type::attribs& attr);
#endif /*	#ifndef _CPF_SYS_COLOUR_CONFIG_H	*/

/*

#define BLOCK_SPACE_TOKEN "^_6r!|"
//http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
#define CURSOR_POS_TOKEN "^@10-20|"

interesting:
http://bjh21.me.uk/all-escapes/all-escapes.txt
*/

//http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
#define CURSOR_POS_TOKEN "/@10-20]"

#define ATTRIBUTE_TOKEN \
	/*bold*/ "/bld]"\
	/*dim*/ "/dim]"\
	/*underline*/ "/uln]"\
	/*blink*/ "/blk]"\
	/*reverse*/ "/rvs]"\
	/*hidden*/ "/hid]"\

#define ATTRIB_COL_TOKEN_EXAMPLES \
	"/&32f]"\
	"/&uln.24f]"\
	"/&dim.75b]"\
	"/&bld.24f.128b]"\
	"/&bld.y!b]"\

/*
	this should find all strings that match exactly those specifed
	and prefix then with a smart formatter.
	*/
#define SUB_STRING_FORMAT_TAGGING\
	"/$mystring|y!]"\
	"/$mystring1;mystring2|r;b!]"\
	"/$mystring1;mystring2|g!]"\
	"/$mystring1;mystring2|bld.r;b!]"
