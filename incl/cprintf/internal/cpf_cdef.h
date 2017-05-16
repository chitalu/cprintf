#ifndef __CPF_CDEF_H__
#define __CPF_CDEF_H__

#include <cprintf/internal/cpf_base.h>
#include <cprintf/internal/cpf_type.h>

#ifdef _WIN32

/*
        auxillary symbols to help shorten code in cpp
        and aid in readability
*/

/*
        *\dim foreground colours
*/
#define CPF_Rf (FOREGROUND_RED)
#define CPF_Gf (FOREGROUND_GREEN)
#define CPF_Bf (FOREGROUND_BLUE)
#define CPF_Yf (CPF_Rf | CPF_Gf)
#define CPF_Mf (CPF_Rf | CPF_Bf)
#define CPF_Cf (CPF_Gf | CPF_Bf)
#define CPF_Wf (CPF_Rf | CPF_Gf | CPF_Bf)

/*
        *\dim background colours
*/
#define CPF_Rb (BACKGROUND_RED)
#define CPF_Gb (BACKGROUND_GREEN)
#define CPF_Bb (BACKGROUND_BLUE)
#define CPF_Yb (CPF_Rb | CPF_Gb)
#define CPF_Mb (CPF_Rb | CPF_Bb)
#define CPF_Cb (CPF_Gb | CPF_Bb)
#define CPF_Wb (CPF_Rb | CPF_Gb | CPF_Bb)

/*
        *\colour intensifiers
*/
#define CPF_fgi (FOREGROUND_INTENSITY)
#define CPF_bgi (BACKGROUND_INTENSITY)

/*
        *\bright foreground colours
*/
#define CPF_Rfi (CPF_Rf | CPF_fgi)
#define CPF_Gfi (CPF_Gf | CPF_fgi)
#define CPF_Bfi (CPF_Bf | CPF_fgi)
#define CPF_Yfi (CPF_Yf | CPF_fgi)
#define CPF_Mfi (CPF_Mf | CPF_fgi)
#define CPF_Cfi (CPF_Cf | CPF_fgi)
#define CPF_Wfi (CPF_Wf | CPF_fgi)

/*
        *\bright background colours
*/
#define CPF_Rbi (CPF_Rb | CPF_bgi)
#define CPF_Gbi (CPF_Gb | CPF_bgi)
#define CPF_Bbi (CPF_Bb | CPF_bgi)
#define CPF_Ybi (CPF_Yb | CPF_bgi)
#define CPF_Mbi (CPF_Mb | CPF_bgi)
#define CPF_Cbi (CPF_Cb | CPF_bgi)
#define CPF_Wbi (CPF_Wb | CPF_bgi)

#endif /*	#ifdef _WIN32	*/

namespace _cprintf_
{

/*
        the full standard/ cross platform set of colour tokens
        that may be used to modify text appearance
*/
CPF_API const _cprintf_::unicode_string_vector_t std_tokens;

/*
        tokens available on both windows and linux
*/
CPF_API const _cprintf_::token_value_storage_t std_token_vals;

}

#endif /*	#ifndef __CPF_COLOUR_DEFS_H__	*/
