/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

*/

#ifndef _CPF_COMMON_H
#define _CPF_COMMON_H

#ifdef _WIN32 /* windows */

/*
 * CPF_BUILD_AS_STATIC is defined for static library.
 * CPF_FUNC_EXPORT  is defined for building the DLL library.
 */

#ifdef CPF_BUILD_AS_STATIC
#  define CPF_API extern
#else
#  ifdef CPF_FUNC_EXPORT
#    define CPF_API extern __declspec(dllexport)
#  else
#    define CPF_API extern __declspec(dllimport)
#  endif
#endif

#define SYSTXTATTIB_UNDEF (0xFF)
 
#else /* UNIX */

#ifdef CPF_BUILD_AS_STATIC
#  define CPF_API extern
#else
#  if defined(__GNUC__) && __GNUC__>=4
#   define CPF_API extern __attribute__ ((visibility("default")))
#  else
#   define CPF_API extern
#  endif
#endif

#define SYSTXTATTIB_UNDEF ("undef")
#endif

#include "_cpf_type.h"

#include <stdexcept>
#include <algorithm>

/*
	
*/
enum class ext_ASCII : unsigned char
{
	euro = 0x80,
	single_low_9_quote_mark = 0x82,
	latin_lcase_f = 0x83,
	double_low_9_quote_mark = 0x84,
	horizontal_ellipsis = 0x85,
	dagger = 0x86,
	double_dagger = 0x87,
	modifier_letter_circumflex_accent = 0x88,
	per_mille_sign = 0x89,
	latin_capital_letter_S_with_caron = 0x8A,
	single_left_pointing_angle_quotation = 0x8B,
	latin_capital_ligature_OE = 0x8C,
	latin_captial_letter_Z_with_caron = 0x8E,
	left_single_quotation_mark = 0x91,
	right_single_quotation_mark = 0x92,
	left_double_quotation_mark = 0x93,
	right_double_quotation_mark = 0x94,
	bullet = 0x95,
	en_dash = 0x96,
	em_dash = 0x97,
	small_tilde = 0x98,
	trade_mark_sign = 0x99,
	Latin_small_letter_S_with_caron = 0x9A,
	single_right_pointing_angle_quotation_mark = 0x9B,
	latin_small_ligature_oe = 0x9C,
	latin_small_letter_z_with_caron = 0x9E,
	latin_capital_letter_Y_with_diaeresis = 0x9F,
	non_breaking_space = 0xA0,
	inverted_exclamation_mark = 0xA1,
	cent_sign = 0xA2,
	pound_sign = 0xA3,
	currency_sign = 0xA4,
	yen_sign = 0xA5,
	broken_vertical_bar = 0xA6,
	section_sign = 0xA7,
	Spacing_diaeresis__umlaut = 0xA8,
	copyright_sign = 0xA9,
	feminine_ordinal_indicator = 0xAA,
	left_double_angle_quotes = 0xAB,
	not_sign = 0xAC,
	soft_hyphen = 0xAD,
	registered_trade_mark_sign = 0xAE,
	Spacing_macron__overline = 0xAF,
	degree_sign = 0xB0,
	plus_or_minus_sign = 0xB1,
	superscript_two__squared = 0xB2,
	superscript_three__cubed = 0xB3,
	acute_accent__spacing_acute = 0xB4,
	micro_sign = 0xB5,
	pilcrow_sign__paragraph_sign = 0xB6,
	middle_dot__Georgian_comma = 0xB7,
	spacing_cedilla = 0xB8,
	superscript_one = 0xB9,
	Masculine_ordinal_indicator = 0xBA,
	right_double_angle_quotes = 0xBB,
	fraction_one_quarter =  0xBC,
	fraction_one_half = 0xBD,
	fraction_three_quarters = 0xBE,
	inverted_question_mark = 0xBF
};

#endif //_CPF_COMMON_H