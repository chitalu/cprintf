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

#ifndef __CPF_CDEF_H__
#define __CPF_CDEF_H__

#include "cprintf/internal/cpf_base.h"

#ifdef _WIN32

/*
	auxillary symbols to help shorten code in cpp
	and aid in readability
*/

/*
	*\dim foreground colours
*/
#define cpf_Rf 	(FOREGROUND_RED)
#define cpf_Gf 	(FOREGROUND_GREEN)
#define cpf_Bf 	(FOREGROUND_BLUE)
#define cpf_Yf		(cpf_Rf | cpf_Gf)
#define cpf_Mf		(cpf_Rf | cpf_Bf)
#define cpf_Cf		(cpf_Gf | cpf_Bf)
#define cpf_Wf		(cpf_Rf | cpf_Gf | cpf_Bf)

/*
	*\dim background colours
*/
#define cpf_Rb 	(BACKGROUND_RED)
#define cpf_Gb 	(BACKGROUND_GREEN)
#define cpf_Bb 	(BACKGROUND_BLUE)
#define cpf_Yb		(cpf_Rb | cpf_Gb)
#define cpf_Mb		(cpf_Rb | cpf_Bb)
#define cpf_Cb		(cpf_Gb | cpf_Bb)
#define cpf_Wb		(cpf_Rb | cpf_Gb | cpf_Bb)

/*
	*\colour intensifiers
*/
#define cpf_fgi 	(FOREGROUND_INTENSITY)
#define cpf_bgi	(BACKGROUND_INTENSITY)

/*
	*\bright foreground colours
*/
#define cpf_Rf_i ( cpf_Rf	| cpf_fgi )
#define cpf_Gf_i ( cpf_Gf	| cpf_fgi )
#define cpf_Bf_i ( cpf_Bf	| cpf_fgi )
#define cpf_Yf_i ( cpf_Yf	| cpf_fgi )
#define cpf_Mf_i ( cpf_Mf	| cpf_fgi )
#define cpf_Cf_i ( cpf_Cf	| cpf_fgi )
#define cpf_Wf_i ( cpf_Wf	| cpf_fgi )

/*
	*\bright background colours 
*/
#define cpf_Rb_i ( cpf_Rb	| cpf_bgi )
#define cpf_Gb_i ( cpf_Gb	| cpf_bgi )
#define cpf_Bb_i ( cpf_Bb	| cpf_bgi )
#define cpf_Yb_i ( cpf_Yb	| cpf_bgi )
#define cpf_Mb_i ( cpf_Mb	| cpf_bgi )
#define cpf_Cb_i ( cpf_Cb	| cpf_bgi )
#define cpf_Wb_i ( cpf_Wb	| cpf_bgi )

#endif /*	#ifdef _WIN32	*/

namespace cpf
{
	namespace intern
	{
		/*
			the full standard/ cross platform set of colour tokens
			that may be used to modify text appearance
		*/
		CPF_API const cpf::type::string_vector std_tokens;

		/*
			tokens available on both windows and linux
		*/
		CPF_API const cpf::type::token_value_map std_token_vals;
	}
}

#endif /*	#ifndef __CPF_COLOUR_DEFS_H__	*/