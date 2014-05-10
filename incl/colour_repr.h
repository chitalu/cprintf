#ifndef _CPF_COLOUR_REPR_H
#define _CPF_COLOUR_REPR_H

#include "common.h"

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

#else /*	#ifdef _WIN32	*/

#endif /*	#ifdef _WIN32	*/

extern const _cpf_types::colour_token_map _cpf_colour_token_vals;
extern const _cpf_types::string_vector _cpf_colour_tokens;
extern const _cpf_types::string_vector _cpf_blockspace_tokens;

#endif /*	#ifndef _CPF_COLOUR_REPR_H	*/

/*

#define BLOCK_SPACE_TOKEN "^_6r!|"
//http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
#define CURSOR_POS_TOKEN "^@10-20|"

interesting:
http://bjh21.me.uk/all-escapes/all-escapes.txt
*/

#define BLOCK_SPACE_TOKEN \
	"/¬6<->=r¬]"\
	"/¬24<foo>=r¬]"\
	"/¬9<bar>=#94f¬]"\
	"/¬10<bar>=rvs;#94b¬]"\
	"/¬10<bar>=rvs;#94f;65b¬]"

//http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
#define CURSOR_POS_TOKEN "/@10-20]"

#define FULL_SPECTRUM_COLOUR_TOKEN \
	"/#252b]"\
	"/#128f]"\
	"/#64f;24b]"\
	"/#2b;28f]"

#define ATTRIBUTE_TOKEN \
	/*bold*/ "/bld]"\
	/*dim*/ "/dim]"\
	/*underline*/ "/uln]"\
	/*blink*/ "/blk]"\
	/*reverse*/ "/rvs]"\
	/*hidden*/ "/hid]"\

#define ATTRIB_COL_TOKEN_EXAMPLES \
	"/&uln;#24f]"\
	"/&dim;#75b]"\
	"/&bld;#24f;#128b]"\
	"/&bld;y!b]"\

/*
	this should find all strings that match exactly those specifed
	and prefix then with a smart formatter.
*/
#define SUB_STRING_FORMAT_TAGGING\
	"/$mystring|(y!)]"\
	"/$mystring1:mystring2|(r);(b!)]"\
	"/$mystring1:mystring2|(g!)]"\
	"/$mystring1:mystring2|(bld);(r);(b!)]"
     
#define CLEAR_SCREEN "/cs]"

/*
parsing steps
1. block space parse
2. sub-string tag parse
3. full spectrm colour token parse (#34f;28b)
4. attibs-fsc parse

for linux implementation a vector hould now be used to hold the
"colour access tokens" allowing for complex tokens to be treated 
as individual elements of the vectors to be later used individually in
order to access the corresponding terminal value.

*/