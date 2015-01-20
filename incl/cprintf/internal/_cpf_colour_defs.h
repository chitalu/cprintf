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

#ifndef __CPF_COLOUR_DEFS_H__
#define __CPF_COLOUR_DEFS_H__

#include "_cpf_common.h"

#ifdef _WIN32

/*
	auxillary symbols to help shorten code in cpp
	and aid in readability
*/

/*
	*\dim foreground colours
*/
#define _cpf_Rf 	(FOREGROUND_RED)
#define _cpf_Gf 	(FOREGROUND_GREEN)
#define _cpf_Bf 	(FOREGROUND_BLUE)
#define _cpf_Yf		(_cpf_Rf | _cpf_Gf)
#define _cpf_Mf		(_cpf_Rf | _cpf_Bf)
#define _cpf_Cf		(_cpf_Gf | _cpf_Bf)
#define _cpf_Wf		(_cpf_Rf | _cpf_Gf | _cpf_Bf)

/*
	*\dim background colours
*/
#define _cpf_Rb 	(BACKGROUND_RED)
#define _cpf_Gb 	(BACKGROUND_GREEN)
#define _cpf_Bb 	(BACKGROUND_BLUE)
#define _cpf_Yb		(_cpf_Rb | _cpf_Gb)
#define _cpf_Mb		(_cpf_Rb | _cpf_Bb)
#define _cpf_Cb		(_cpf_Gb | _cpf_Bb)
#define _cpf_Wb		(_cpf_Rb | _cpf_Gb | _cpf_Bb)

/*
	*\colour intensifiers
*/
#define _cpf_fgi 	(FOREGROUND_INTENSITY)
#define _cpf_bgi	(BACKGROUND_INTENSITY)

/*
	*\bright foreground colours
*/
#define _cpf_Rf_i ( _cpf_Rf	| _cpf_fgi )
#define _cpf_Gf_i ( _cpf_Gf	| _cpf_fgi )
#define _cpf_Bf_i ( _cpf_Bf	| _cpf_fgi )
#define _cpf_Yf_i ( _cpf_Yf	| _cpf_fgi )
#define _cpf_Mf_i ( _cpf_Mf	| _cpf_fgi )
#define _cpf_Cf_i ( _cpf_Cf	| _cpf_fgi )
#define _cpf_Wf_i ( _cpf_Wf	| _cpf_fgi )

/*
	*\bright background colours 
*/
#define _cpf_Rb_i ( _cpf_Rb	| _cpf_bgi )
#define _cpf_Gb_i ( _cpf_Gb	| _cpf_bgi )
#define _cpf_Bb_i ( _cpf_Bb	| _cpf_bgi )
#define _cpf_Yb_i ( _cpf_Yb	| _cpf_bgi )
#define _cpf_Mb_i ( _cpf_Mb	| _cpf_bgi )
#define _cpf_Cb_i ( _cpf_Cb	| _cpf_bgi )
#define _cpf_Wb_i ( _cpf_Wb	| _cpf_bgi )

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