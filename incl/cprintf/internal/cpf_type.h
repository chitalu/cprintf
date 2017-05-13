#ifndef __CPF_TYPE_H__
#define __CPF_TYPE_H__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <array>
#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <cprintf/internal/cpf_base.h>

namespace cpf
{
namespace type
{
// Native string type
typedef std::wstring str_t;

typedef std::string nstr_t;

typedef const wchar_t* cstr;

typedef str_t::size_type  size;
typedef nstr_t::size_type nsize;

#ifdef _WIN32
typedef WORD colour;
#else
typedef str_t colour;
#endif

typedef std::pair<str_t, str_t> str_pair_t;
typedef std::vector<str_t> str_vec_t;
typedef str_vec_t          attribute_group;
// format string in its meta form i.e. split state
typedef std::map<cpf::type::size, std::pair<str_vec_t, str_t> > meta_fmt_t;
typedef cpf::type::meta_fmt_t::iterator       meta_iterator;
typedef cpf::type::meta_fmt_t::const_iterator c_meta_iterator;
typedef std::map<const str_t, cpf::type::colour> token_value_map;
typedef decltype(stdout) stream_t;

// placeholder type
struct stub_t
{
};

template <bool B>
using get_btype_ = std::conditional<B, std::true_type, std::false_type>;

template <typename T>
struct is_nchar_ptr_
  : get_btype_<std::is_same<char*, T>::value ||
               std::is_same<unsigned char*, T>::value ||
               std::is_same<signed char*, T>::value ||
               std::is_same<const char*, T>::value ||
               std::is_same<const signed char*, T>::value ||
               std::is_same<const unsigned char*, T>::value>::type
{
};

template <typename T>
struct is_wchar_ptr_t : get_btype_<std::is_same<wchar_t*, T>::value ||
                                   std::is_same<const wchar_t*, T>::value>::type
{
};

template <typename T>
struct is_char_ptr_t
  : get_btype_<is_nchar_ptr_<T>::value || is_wchar_ptr_t<T>::value>::type
{
};

template <typename T>
struct is_nstype_ : get_btype_<std::is_same<T, std::string>::value ||
                               is_nchar_ptr_<T>::value>::type
{
};

template <typename T>
struct is_wstype_ : get_btype_<std::is_same<T, std::wstring>::value ||
                               is_wchar_ptr_t<T>::value>::type
{
};

template <typename T = stub_t>
struct is_permitted_string_type_
  : get_btype_<is_wstype_<T>::value || is_nstype_<T>::value>::type
{
};

template <typename T = stub_t>
struct is_STL_string_type_ : get_btype_<std::is_same<T, nstr_t>::value ||
                                        std::is_same<T, str_t>::value>::type
{
};

template <typename T = str_t>
struct verify_format_
{
	typedef T type;
	static_assert(is_permitted_string_type_<T>::value,
	              "cprintf error: invalid format type");
};


template <typename T = int, typename... Ts>
struct verify_args_
{
	typedef T current_type;

	typedef
	  typename get_btype_<is_permitted_string_type_<current_type>::value ||
	                      (std::is_arithmetic<current_type>::value ||
	                       std::is_pointer<current_type>::value)>::type current;

	typedef verify_args_<Ts...> Next;

	static_assert(current::value && Next::current::value,
	              "cprintf error: type not allowed");
};
}
}

#endif
