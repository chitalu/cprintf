/*

Copyright (C) 2017 Floyd Mulenga Chitalu

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

#pragma once

#if defined(_WIN64) || defined(_WIN32)

#define CPF_WINDOWS_BUILD 1

#elif defined(__APPLE__)

#define CPF_MAC_BUILD 1

#elif defined(__linux__) || defined(__unix__)

#define CPF_LINUX_BUILD 1

#endif // #if defined(_WIN64) || defined(_WIN32)

#if !defined(CPF_DBG_CONFIG) //... in compiler flags

#ifndef NDEBUG

#define CPF_DBG_CONFIG 1

#else

#define CPF_DBG_CONFIG 0

#endif // NDEBUG

#endif /* !defined(CPF_DBG_CONFIG) */

#include <map>
#include <mutex>
#include <vector>
#include <cstdint>
#include <string>

#ifdef CPF_WINDOWS_BUILD
#include <Windows.h>
#ifndef CPF_BUILD_AS_SHARED

#define CPF_API extern

#else

#ifdef CPF_FUNC_EXPORT
#define CPF_API extern __declspec(dllexport)
#else
#define CPF_API extern __declspec(dllimport)
#endif // #ifdef CPF_FUNC_EXPORT

#endif // #ifdef CPF_BUILD_AS_SHARED

#elif defined(CPF_LINUX_BUILD) /*| defined(CPF_MAC_BUILD)*/

#ifndef CPF_BUILD_AS_SHARED
#define CPF_API extern
#else
#ifdef CPF_FUNC_EXPORT
#if defined(__GNUC__) && __GNUC__ >= 4
#define CPF_API extern __attribute__((visibility("default")))
#else
#pragma warning Unknown dynamic link import / export semantics.
#endif
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define CPF_API
#else
#pragma warning Unknown dynamic link import / export semantics.
#endif // #if defined (__GNUC__) && __GNUC__>=4
#endif // #ifdef CPF_FUNC_EXPORT
#endif // #ifdef CPF_BUILD_AS_SHARED

#endif /*#ifdef CPF_WINDOWS_BUILD*/

/*
error codes.
*/

// no error
#define CPF_SUCCESS (0)

// invalid token(s) encountered in format string
#define CPF_FORMAT_STRING_TOKEN_ERROR (-1)

// invalid format specifier(s) in format string
#define CPF_FORMAT_SPECIFIER_ERROR (-2)

// mismatch between a format specifier and its corresponding argument
#define CPF_ARG_ERR (-3)

// system config error
#define CPF_SYSTEM_ERR (-4)

// ascii to unicode character string conversion error.
#define CPF_NWCONV_ERR (-5)

#define CPF_ESC_CHAR ('`')

// from narrow to wide e.g. const char* -> const wchar_t*
// string literals only.
#define CPF_WIDEN_2__(x) L##x
#define CPF_WIDEN_1__(x) CPF_WIDEN_2__(x)
#define CPF_WIDEN_STRING_LITERAL(s) CPF_WIDEN_1__(s)

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

namespace _cprintf_ {
typedef std::string unicode_string_t;

typedef std::string ascii_string_t;

#ifdef CPF_WINDOWS_BUILD
typedef WORD system_color_repr_t;
#else
typedef unicode_string_t system_color_repr_t;
#endif


typedef const char *unicode_character_string_ptr_t;

typedef std::pair<unicode_string_t, unicode_string_t> unicode_string_pair_t;

typedef std::vector<unicode_string_t> unicode_string_vector_t;

typedef unicode_string_vector_t attribute_group_t;

typedef std::map<int, std::pair<unicode_string_vector_t, unicode_string_t>>
    format_string_layout_t;

typedef format_string_layout_t::iterator format_string_layout_iterator_t;

typedef format_string_layout_t::const_iterator
    format_string_layout_const_iterator_t;

typedef std::map<const unicode_string_t, system_color_repr_t>
    token_value_storage_t;

typedef decltype(stdout) file_stream_t;

// placeholder type
struct stub_t {};

template <bool B>
using infer_boolean_type_ =
    std::conditional<B, std::true_type, std::false_type>;

template <typename T>
struct is_ascii_char_ptr_
    : infer_boolean_type_<std::is_same<char *, T>::value ||
                          std::is_same<unsigned char *, T>::value ||
                          std::is_same<signed char *, T>::value ||
                          std::is_same<const char *, T>::value ||
                          std::is_same<const signed char *, T>::value ||
                          std::is_same<const unsigned char *, T>::value>::type {
};

template <typename T>
struct is_unicode_char_ptr_t
    : infer_boolean_type_<std::is_same<wchar_t *, T>::value ||
                          std::is_same<const wchar_t *, T>::value>::type {};

template <typename T>
struct is_ascii_string_type
    : infer_boolean_type_<std::is_same<T, std::string>::value ||
                          is_ascii_char_ptr_<T>::value>::type {};

template <typename T>
struct is_unicode_string_type
    : infer_boolean_type_<std::is_same<T, std::wstring>::value ||
                          is_unicode_char_ptr_t<T>::value>::type {};

template <typename T = stub_t>
struct is_valid_string_type_
    : infer_boolean_type_<is_ascii_string_type<T>::value>::type {};

template <typename T = stub_t>
struct is_STL_string_type_
    : infer_boolean_type_<std::is_same<T, ascii_string_t>::value ||
                          std::is_same<T, unicode_string_t>::value>::type {};

template <typename T = int, typename... Ts> struct verify_args_ {
  typedef T current_type;

  typedef
      typename infer_boolean_type_<is_valid_string_type_<current_type>::value ||
                                   (std::is_arithmetic<current_type>::value ||
                                    std::is_pointer<current_type>::value)>::type
          current;

  typedef verify_args_<Ts...> Next;

  static_assert(current::value && Next::current::value,
                "invalid argument type");
};

// verifies that the format string contains arguments which	match the given
// % sequence(s) in the correct order i.e "%d" must correspond to an integral,
// "%p" to a pointer etc.. note that this is only able to test those format
// specifiers found in "std_fmt_specs".

CPF_API void
format_specifier_correspondence_check(unicode_character_string_ptr_t format);

template <class T = int, typename... Ts>
void format_specifier_correspondence_check(
    unicode_character_string_ptr_t format, T &&farg, Ts &&... args) {
  for (; *format; ++format) {
    if (*format != '%' || *++format == '%')
      continue;

    char f = *format;

    switch (f) {
    case 'f':
    case 'e':
    case 'g': {
      if (!std::is_floating_point<T>::value) {
        throw CPF_ARG_ERR; // expected a[floating point] value
      }
    } break;
    case 'd':
    case 'i':
    case 'o':
    case 'u':
    case 'c':
    case 'x':
    case 'l': // note that this is actually in "inter_fmt_specs"
    case '#': // note that this is actually in "inter_fmt_specs"
    {
      if (!std::is_integral<T>::value) {
        throw CPF_ARG_ERR; // expected an[integral] value"
      }
    } break;
    case 's':
    case 'S': {
      if (!is_valid_string_type_<T>::value) {
        throw CPF_ARG_ERR; // expected a value of type[c - string, std::string
                           // or std::wstring]
      }
    } break;
    case 'p': {
      if (!std::is_pointer<T>::value) {
        throw CPF_ARG_ERR; // expected a[pointer] value
      }
    } break;
    default:
      break; //	Note: we do not cover all edge cases
    }

    return format_specifier_correspondence_check(++format,
                                                 std::forward<Ts>(args)...);
  }
}

//	the full standard / cross platform set of colour tokens	that may be
// used to modify text appearance
CPF_API const unicode_string_vector_t std_tokens;

// tokens available on both windows and linux
CPF_API const token_value_storage_t std_token_vals;

CPF_API bool runningInSystemTerminal();

CPF_API void save_stream_state(file_stream_t ustream);

// boolean finished_cpf_exec is to indicate whether this is the last call to
// before cprintf finishes execution to return by default this function executes
// only if save_stream_state has been previous called (in the current call to
// cprintf) else it returns immidiately. This behaviour can be overridden via
// force_restore.
CPF_API void restore_stream_state(file_stream_t ustream,
                                  bool finished_cpf_exec = false);

CPF_API void configure(file_stream_t ustream, const attribute_group_t &attr);

CPF_API const unicode_string_t debugging_log_format_string;

//	text attribute token escape sequences..
CPF_API const std::initializer_list<unicode_string_t> attr_esc_seqs;

// standard format specifiers
//%c print a singlecharacter
//%d print a decimal (base 10) number
//%e print an exponential floating-point number
//%f print a floating-point number
//%g print a general-format floating-point number
//%i print an integer in base 10
//%o print a number in octal (base 8)
//%s print a string of characters
//%u print an unsigned decimal (base 10) number
//%x print a number in hex idecimal (base 16)
//%% print a percent sign (\% also works)
CPF_API const std::initializer_list<char> std_fmt_specs;

// extended format specifier terminators	'd', 'f', 's', 'e', 'o', 'x'
CPF_API const std::initializer_list<char> ext_fmtspec_terms;

// 	intermediate format specifiers	'+', '-', '.', '*', '#', 'l'
CPF_API const std::initializer_list<char> inter_fmt_specs;

// \a audible alert (bell) \b backspace	\f form feed \n newline (linefeed)
// \r carriage return \t tab \v vertical tab
CPF_API const std::initializer_list<char> escape_characters;

CPF_API format_string_layout_t
parse_format_string(const unicode_string_t &format_string);

CPF_API int search_for(const unicode_string_t &_what,
                       const unicode_string_t &_where, const int _offset = 0,
                       const char &_esc_char = CPF_ESC_CHAR);

CPF_API unicode_string_t
ascii_to_unicode_string_conversion(unicode_string_t &&src);

CPF_API int
get_num_format_specifiers_in_string(const unicode_string_t &unicode_string_t);

CPF_API unicode_string_t write_substring_before_format_specifier(
    file_stream_t file_stream, unicode_string_t &printed_string_, int &ssp_,
    const attribute_group_t attr);
CPF_API void write_substring_after_format_specifier(
    file_stream_t file_stream, unicode_string_t &printed_string_, int &ssp_,
    bool &more_args_on_iter,
    format_string_layout_t::const_iterator &format_string_layout_iterator,
    const format_string_layout_t::const_iterator &end_point_comparator);

CPF_API void write_substring_without_format_specifier(
    file_stream_t file_stream, unicode_string_t &printed_string_, int &ssp_,
    format_string_layout_t::const_iterator &format_string_layout_iterator);

CPF_API unicode_string_t
resolve_string_type_format_specifier(const unicode_string_t &fs);

template <typename T>
void write_variadic_argument_to_file_stream(file_stream_t file_stream,
                                            unicode_string_t const &format,
                                            T &&arg) {
  const unicode_string_t f = resolve_string_type_format_specifier(format);
  std::fprintf(file_stream, f.c_str(), arg);
}

template <>
void write_variadic_argument_to_file_stream<unicode_string_t>(
    file_stream_t file_stream, unicode_string_t const &format,
    unicode_string_t &&arg);

CPF_API void print_format_string_layout(
    file_stream_t file_stream,
    const format_string_layout_const_iterator_t &end_point_comparator,
    format_string_layout_const_iterator_t &format_string_layout_iterator,
    const unicode_string_t printed_string, const int search_start_pos);

template <typename ArgType0, typename... RemainingArgTypes>
void print_format_string_layout(
    file_stream_t file_stream,
    const format_string_layout_const_iterator_t &end_point_comparator,
    format_string_layout_const_iterator_t &format_string_layout_iterator,
    const unicode_string_t printed_string, const int search_start_pos,
    ArgType0 &&arg0, RemainingArgTypes &&... args) {
  unicode_string_t printed_string_ = printed_string;
  // printed string argument-specifier ('%') count
  const auto pstr_argc = get_num_format_specifiers_in_string(printed_string);

  // more variadic args to write using "printed_string_" as format string.
  bool more_args_on_iter = false,
       // boolean used to signify whether variadic "arg0" has been passed to
      // std::fprintf as an argument yet
      printed_arg0 = false;

  // string parsing start position...
  auto ssp_ = search_start_pos;

  if (pstr_argc >= 1) {
    const unicode_string_t argument_format_specifier =
        write_substring_before_format_specifier(
            file_stream, printed_string_, ssp_,
            format_string_layout_iterator->second.first);

    write_variadic_argument_to_file_stream(
        file_stream, argument_format_specifier, std::forward<ArgType0>(arg0));

    write_substring_after_format_specifier(
        file_stream, printed_string_, ssp_, more_args_on_iter,
        format_string_layout_iterator, end_point_comparator);
    printed_arg0 = true;
  } else {
    write_substring_without_format_specifier(file_stream, printed_string_, ssp_,
                                             format_string_layout_iterator);
  }

  const bool iter_reached_end =
      (format_string_layout_iterator == end_point_comparator);
  const unicode_string_t i_raw_str =
      iter_reached_end ? "" : format_string_layout_iterator->second.second;

  // only when "arg0" has been passed to std::fprintf does variadic-argument
  // based recursion proceed onto the next one subsequently after arg0. Else
  // recurse back into this function with the same arguments.
  if (printed_arg0) {
    print_format_string_layout(
        file_stream, end_point_comparator, format_string_layout_iterator,
        (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
        more_args_on_iter, std::forward<RemainingArgTypes>(args)...);
  } else {
    print_format_string_layout(
        file_stream, end_point_comparator, format_string_layout_iterator,
        (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
        ssp_, std::forward<ArgType0>(arg0),
        std::forward<RemainingArgTypes>(args)...);
  }
}

template <typename FormatType, typename... ArgumentTypes>
int dispatch(file_stream_t file_stream, FormatType &&format,
             ArgumentTypes &&... args) {
  using namespace _cprintf_;

  static_assert(is_valid_string_type_<FormatType>::value,
                "invalid format string type");

  static_assert(verify_args_<ArgumentTypes...>::current::value, "invalid args");

  int error_code = CPF_SUCCESS;

  try {

#if CPF_DBG_CONFIG
    format_specifier_correspondence_check(
        std::forward<unicode_character_string_ptr_t>(unicode_string_t(format).c_str()),
        std::forward<ArgumentTypes>(args)...);
#endif

    save_stream_state(file_stream);

    format_string_layout_t format_string_layout = parse_format_string(format);
    format_string_layout_const_iterator_t format_string_layout_begin_iterator =
        format_string_layout.cbegin();
    format_string_layout_const_iterator_t format_string_layout_end_iterator =
        format_string_layout.cend();

    print_format_string_layout(
        file_stream, format_string_layout_end_iterator,
        format_string_layout_begin_iterator,
        format_string_layout_begin_iterator->second.second, 0u,
        std::forward<ArgumentTypes>(args)...);
  } catch (int thrown_error_code) {
    error_code = thrown_error_code;
  }

  restore_stream_state(file_stream, true);

  return error_code;
}
}

template <typename... Types>
inline int cprintf(decltype(stdout) stream, Types... arguments) {
  return _cprintf_::dispatch(stream, std::forward<Types>(arguments)...);
}

template <typename... Types> inline int cprintf(Types... arguments) {
  return cprintf(stdout, std::forward<Types>(arguments)...);
}

template <typename FormatType, unsigned N, typename... Types>
inline int cprintf_s(FormatType (&format)[N], Types... arguments) {
  static_assert(N >= 2, "invalid string-literal");
  return cprintf(format, std::forward<Types>(arguments)...);
}

#if CPF_DBG_CONFIG

#include <cstdarg>

#ifdef CPF_WINDOWS_BUILD
#define CPF_FILE_PATH_SEPARATOR(character)                                     \
  (character == '\\' || character == '/');
#else
#define CPF_FILE_PATH_SEPARATOR(character) (character == '/');
#endif

#endif

#if CPF_DBG_CONFIG
#define cprintf_dbg(format_, ...)                                              \
  do {                                                                         \
    const _cprintf_::unicode_string_t pname__ =                                \
        CPF_WIDEN_STRING_LITERAL(__FILE__);                                    \
    typedef struct {                                                           \
      bool operator()(char character) const {                                  \
        return CPF_FILE_PATH_SEPARATOR(character)                              \
      }                                                                        \
    } sep__;                                                                   \
    auto fname__ = unicode_string_t(                                           \
        std::find_if(pname__.rbegin(), pname__.rend(), sep__()).base(),        \
        pname__.end());                                                        \
    cprintf(stderr, debugging_log_format_string, fname__, __TIME__, __DATE__,  \
            __FUNCTION__, __LINE__);                                           \
    cprintf(stderr, format_, ##__VA_ARGS__);                                   \
  } while (false)
#else
#define cprintf_dbg(format, ...)
#endif

extern "C" {
CPF_API int cprintf_capi(int stream, const char * format);
};
