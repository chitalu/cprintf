#ifndef _CPF__cpf_sys_colour_config_H
#define _CPF__cpf_sys_colour_config_H

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

extern "C" _cpf_types::attributes _cpf_current_text_attribs;
extern "C" const _cpf_types::string_vector _cpf_std_tokens;

extern const _cpf_types::colour_token_map _cpf_colour_token_vals;
extern "C" const _cpf_types::string_vector _cpf_std_tokens;
extern const _cpf_types::string_vector _cpf_blockspace_tokens;

/*
	configure system terminal settings

	@strm 	- output stream
	@attr - attribute vector
*/
extern "C" void _cpf_config_terminal(_cpf_types::stream strm,
	const _cpf_types::attributes& attr);

#endif /*	#ifndef _CPF__cpf_sys_colour_config_H	*/

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

/*
parsing steps
2. sub-string tag parse
3. full spectrm colour token parse (#34f;28b)
4. attibs-fsc parse
*/