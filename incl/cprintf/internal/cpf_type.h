#ifndef __CPF_TYPE_H__
#define __CPF_TYPE_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <inttypes.h>
#include <cstdint>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <array>

#include <cprintf/internal/cpf_base.h>

namespace cpf {
namespace type {
// Native string type
typedef std::wstring str_t;

typedef std::string nstr_t;

typedef const wchar_t *cstr;

typedef str_t::size_type size;
typedef nstr_t::size_type nsize;

#ifdef _WIN32
typedef WORD colour;
#else
typedef str_t colour;
#endif

typedef std::pair<str_t, str_t> str_pair_t;
typedef std::vector<str_t> str_vec_t;
typedef str_vec_t attribute_group;
// format string in its meta form i.e. split state
typedef std::map<cpf::type::size, std::pair<str_vec_t, str_t> > meta_fmt_t;
typedef cpf::type::meta_fmt_t::iterator meta_iterator;
typedef cpf::type::meta_fmt_t::const_iterator c_meta_iterator;
typedef std::map<const str_t, cpf::type::colour> token_value_map;

template <typename... Ts> using arg_pack_t = std::tuple<Ts...>;

typedef FILE *stream_t;

typedef int rcode_t;

// placeholder type
struct stub_t {};

template <bool B>
using get_btype_ = std::conditional<B, std::true_type, std::false_type>;

// wide character pointer (signed, unsigned, const, and non-const)
template <typename T>
struct is_nchar_ptr_
    : get_btype_<std::is_same<char *, T>::value ||
                 std::is_same<unsigned char *, T>::value ||
                 std::is_same<signed char *, T>::value ||
                 std::is_same<const char *, T>::value ||
                 std::is_same<const signed char *, T>::value ||
                 std::is_same<const unsigned char *, T>::value>::type {};

// wide character pointer (const and non-const)
template <typename T>
struct is_wchar_ptr_t
    : get_btype_<std::is_same<wchar_t *, T>::value ||
                 std::is_same<const wchar_t *, T>::value>::type {};

// signifies a char pointer type (wide or narrow)
template <typename T>
struct is_char_ptr_t
    : get_btype_<is_nchar_ptr_<T>::value || is_wchar_ptr_t<T>::value>::type {};

// check if T is a narrow character string type
template <typename T>
struct is_nstype_ : get_btype_<std::is_same<T, std::string>::value ||
                               is_nchar_ptr_<T>::value>::type {};

// check if T is a wide character string type
template <typename T>
struct is_wstype_ : get_btype_<std::is_same<T, std::wstring>::value ||
                               is_wchar_ptr_t<T>::value>::type {};

// checks if the user given format string is of a permitted type
template <typename T = stub_t>
struct is_permitted_string_type_
    : get_btype_<is_wstype_<T>::value || is_nstype_<T>::value>::type {};

template <typename T = stub_t>
struct is_STL_string_type_ : get_btype_<std::is_same<T, nstr_t>::value ||
                                        std::is_same<T, str_t>::value>::type {};

// helper struct used to define the format string representative STL type.
// this type can either be std::string or std::wstring
template <typename T>
struct get_STL_string_type_
    : std::conditional<is_wstype_<T>::value, str_t, nstr_t> {};


template <typename T = str_t> struct verify_format_ {
  typedef T type;
  static_assert(is_permitted_string_type_<T>::value,
                "CPRINTF COMPILATION ERROR: Invalid format string type");
};

template <typename T> struct get_capture_type_ {
  typedef typename std::conditional<is_wstype_<T>::value, str_t, nstr_t>::type
  type;
};

template <typename T = int, typename... Ts> struct verify_args_ {
  typedef T current_type;

  typedef typename get_btype_ <
          is_permitted_string_type_<current_type>::value ||
      (std::is_arithmetic<current_type>::value ||
       std::is_pointer<current_type>::value) > ::type current;

  typedef verify_args_<Ts...> Next;

  static_assert(current::value && Next::current::value,
                "CPRINTF COMPILATION ERROR: argument type not allowed");
};

template <typename T> struct resolve_Xint64_t_ {
  typedef typename std::conditional<std::is_signed<T>::value, std::int64_t,
                                    std::uint64_t>::type type;
};

template <typename T> struct xpromote_ {
  verify_args_<T> decl_stub_;

  typedef typename std::conditional<
      std::is_integral<T>::value, typename resolve_Xint64_t_<T>::type,
      typename std::conditional<
          std::is_floating_point<T>::value, double,
          typename std::conditional<
              std::is_pointer<T>::value, T,
              cpf::type::stub_t // used to generate compilde time error
              >::type>::type>::type type;
};

// API return type holds the return code signifying the status
// of a particular invocation. The return code can be a value
// set to any of the possible values defined in cpf_base.h
// As an auxilliary feature this type also returns the format string
// specified by the user.
template <typename DETAIL> struct status_t {
public:
  // the original Type of the format string as specified by client
  typedef typename DETAIL::fmt::type ftype_t;

  // user format string
  typename get_STL_string_type_<typename DETAIL::fmt::type>::type f;

  // [conditional] captured output ...
  typename std::conditional<
      (DETAIL::flags::get::value &CPF_CAPTURE) == CPF_CAPTURE,
      typename get_capture_type_<typename DETAIL::fmt::type>::type,
      stub_t>::type f_;

  // return code i.e. status of API invocation upon return
  rcode_t c;

  inline bool operator==(const rcode_t &rhs) { return (c == rhs); }
};

}
}

#endif
