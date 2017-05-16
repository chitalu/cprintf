#ifndef __CPF_PARSE_H__
#define __CPF_PARSE_H__

#include "cprintf/internal/cpf_type.h"

namespace _cprintf_
{

/*
        text attribute token escape sequences..
*/
CPF_API const std::initializer_list<unicode_string_t> attr_esc_seqs;

/*
        standard format specifiers

        %c print a singlecharacter
        %d print a decimal (base 10) number
        %e print an exponential floating-point number
        %f print a floating-point number
        %g print a general-format floating-point number
        %i print an integer in base 10
        %o print a number in octal (base 8)
        %s print a string of characters
        %u print an unsigned decimal (base 10) number
        %x print a number in hex idecimal (base 16)
        %% print a percent sign (\% also works)

        %b print an arithmetic or pointer type as binary (custom extension)
*/
CPF_API const std::initializer_list<wchar_t> std_fmt_specs;

/*
        extended format specifier terminators
        'd', 'f', 's', 'e', 'o', 'x'
*/
CPF_API const std::initializer_list<wchar_t> ext_fmtspec_terms;

/*
        intermediate format specifiers
        '+', '-', '.', '*', '#', 'l'
        */
CPF_API const std::initializer_list<wchar_t> inter_fmt_specs;

/*

\a audible alert (bell)
\b backspace
\f form feed
\n newline (linefeed)
\r carriage return
\t tab
\v vertical tab

*/
CPF_API const std::initializer_list<wchar_t> escape_characters;

/*
        As indicated by name, this function takes the source
        format string which is then subsequently passed through various
        parsing stages. On return, the function yeilds a map of [key] type
        std::size_t and [value] type <string-vector : string> pair. The [key]
        represents the positional offset, within the source format
        string, of the [second]-element-of [value].
        The [value] is one in which the [first] element is a vector of
        symbolic string tokens the [second] element is to be
        formatted with.
        The second element of [value] is a substring of the source format
   string.
        */
CPF_API _cprintf_::format_string_layout_t
process_format_string(const unicode_string_t &format_string);
}


#endif
