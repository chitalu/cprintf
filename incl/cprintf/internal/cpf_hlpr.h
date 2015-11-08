#ifndef __CPF_HLPR_H__
#define __CPF_HLPR_H__

#include <tuple>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <bitset>
#include <cwctype> //iswalpha
#include <clocale>

#include <mutex>

#ifdef CPF_WINDOWS_BUILD
// Note:	GCC does not yet support multi-byte conversion functionality
// from the following header. As a result, narrow-character strings
// will not be supported until this is resolved.
#include <codecvt> //wstring_convert
#endif
#include <cstdint>

#include <cprintf/internal/cpf_scan.h>
#include <cprintf/internal/cpf_carg.h>
#include <cprintf/internal/cpf_tconf.h>

namespace cpf {
namespace intern {

using namespace cpf::type;

CPF_API std::mutex mtx_;

template <std::size_t...> struct indices {};

template <std::size_t Begin, std::size_t End, typename Indices = indices<>,
          typename Enable = void>
struct make_seq_indices {
  static_assert(Begin <= End, "Begin must be <= End");
};

template <std::size_t Begin, std::size_t End, template <std::size_t...> class I,
          std::size_t... Indices>
    struct make_seq_indices < Begin,
    End, I<Indices...>, typename std::enable_if<Begin<End, void>::type> {
  using type =
      typename make_seq_indices<Begin + 1, End, I<Indices..., Begin> >::type;
};

template <std::size_t Begin, std::size_t End, typename Indices>
struct make_seq_indices<Begin, End, Indices,
                        typename std::enable_if<Begin == End, void>::type> {
  using type = Indices;
};

// to obtain a list of indices.The type alias is defined as :
template <std::size_t Begin, std::size_t End>
using make_seq_indices_T = typename make_seq_indices<Begin, End>::type;

// It is useful to consider how to pass a set of function arguments to a
// function or functor.The code to do this is:

//**constexpr
template <typename Op, typename... Args>
inline auto apply(Op &&op, Args &&... args)
    -> decltype(std::forward<Op>(op)(std::forward<Args>(args)...)) {
  return std::forward<Op>(op)(std::forward<Args>(args)...);
}

// This function overload applies op to all tuple indices...
template <typename Op, typename Tuple, template <std::size_t...> class I,
          std::size_t... Indices>
inline auto _apply_tuple(Op &&op, Tuple &&t, I<Indices...> &&)
    -> decltype(
          std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...)) {
  return std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...);
}

// This function overload forwards op and t along with the indices of the tuple
// generated by make_seq_indices...
template <typename Op, typename Tuple,
          typename Indices = make_seq_indices_T<
              0, std::tuple_size<typename std::decay<Tuple>::type>::value> >
inline auto apply_tuple(Op &&op, Tuple &&t)
    -> decltype(_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t),
                             Indices{})) {
  return _apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{});
}

// convert from narrow character string to wide character string @returns wide
// version of src
CPF_API cpf::type::str_t wconv(cpf::type::nstr_t &&src);

// pass through ...
CPF_API cpf::type::str_t wconv(cpf::type::str_t &&src);

// @returns number of printf argument tokens "%" in a given string
CPF_API cpf::type::size get_num_arg_specs(const cpf::type::str_t &str_t);

// print the substring preceding an argument specifier in a sub-format-string
CPF_API cpf::type::str_t
write_pre_arg_str(cpf::type::stream_t ustream,
                  cpf::type::str_t &printed_string_, cpf::type::size &ssp_,
                  const cpf::type::attribute_group attr);

// print the substring proceding an argument specifier in sub-format-string
CPF_API void write_post_arg_str(
    cpf::type::stream_t ustream, cpf::type::str_t &printed_string_,
    cpf::type::size &ssp_, bool &more_args_on_iter,
    cpf::type::meta_fmt_t::const_iterator &meta_iter,
    const cpf::type::meta_fmt_t::const_iterator &end_point_comparator);

// print non-argument specifying format string i.e where the implmentation need
// not invoke printf with any avariadic arguments.
CPF_API void
write_non_arg_str(cpf::type::stream_t ustream,
                  cpf::type::str_t &printed_string_, cpf::type::size &ssp_,
                  cpf::type::meta_fmt_t::const_iterator &meta_iter);

CPF_API cpf::type::str_t resolve_str_frmt_spec(const cpf::type::str_t &fs);

// partition the bitfield in into sections of for bits to aid in readability
void partition_bitfield(std::string &str_repr);

template <typename T>
std::string organise_bits(
    typename std::enable_if<!std::is_arithmetic<T>::value, T>::type x) {
  x;
  return std::string();
}

template <typename T>
std::string organise_bits(typename std::enable_if<
    std::is_unsigned<T>::value || std::is_same<std::uintptr_t, T>::value,
    T>::type &val) {

  std::bitset<sizeof(T) * 8U> bits(val);

  auto str_repr = bits.to_string();
  partition_bitfield(str_repr);
  return str_repr;
}

void shift_chars(std::string &v, std::int32_t base, std::int32_t upto,
                 std::int32_t amount);

template <typename T>
std::string
organise_bits(typename std::enable_if<std::is_signed<T>::value, T>::type &val) {
  std::string o;

  std::bitset<sizeof(T) * 8U> bits(val);

  auto str_repr = bits.to_string();
  partition_bitfield(str_repr);

  o.resize(str_repr.length() + 3);
  memcpy(&o[0], str_repr.data(), str_repr.size());

  shift_chars(o, 0, str_repr.length(), 1);
  o[0] = '[';

  shift_chars(o, 2, str_repr.length() + 1, 1);
  o[2] = ']';
  o[o.length() -1] = '\0';

  return o;
}

template <typename T>
std::string organise_bits(
    typename std::enable_if<std::is_floating_point<T>::value, T>::type val) {
  printf("ITS A FLOAT!!\n");
}

template <typename T>
void write_binary(cpf::type::stream_t ustream,
                  typename std::enable_if<std::is_arithmetic<T>::value ||
                                              std::is_pointer<T>::value,
                                          T>::type &&arg) {
  using namespace cpf::type;
  typedef typename std::conditional<std::is_pointer<T>::value, std::uintptr_t,
                                    T>::type T_;

  T_ arg_ = (T_)arg;
  std::string bit_str = organise_bits<T_>(arg_);

  // std::bitset<sizeof(T) * 8U> bits((ptype)(arg));
  // std::string bstr = bits.to_string();
  // std::vector<char> bvec;
  // std::uint32_t c(0U);
  //
  // for (auto &bit : bstr) {
  //   if (!(c % 4) && c) {
  //     bvec.push_back('.');
  //   }
  //   bvec.push_back(bit);
  //   ++c;
  // }
  // bvec.push_back('\0');
  // bstr = bvec.data();
  std::fprintf(ustream, "%s", bit_str.c_str());
}

// Enabled if "T" is an STL string type.
// this function is only instantiated but never executed
template <typename T>
void
write_binary(cpf::type::stream_t ustream,
             typename std::enable_if<cpf::type::is_STL_string_type_<T>::value,
                                     T>::type &&arg) {}

template <typename T>
void write_arg(cpf::type::stream_t ustream, cpf::type::str_t const &format,
               T &&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;

  if (format == L"%b")
    write_binary<T>(ustream, std::forward<T>(arg));
  else {
    str_t f = resolve_str_frmt_spec(format);
    // write argument
    std::fwprintf(ustream, f.c_str(), arg);
  }
}

// extra wizardry has to be performed before printing the
// following types...
template <>
void write_arg<cpf::type::str_t>(cpf::type::stream_t ustream,
                                 cpf::type::str_t const &format,
                                 cpf::type::str_t &&arg);

template <>
void write_arg<cpf::type::nstr_t>(cpf::type::stream_t ustream,
                                  cpf::type::str_t const &format,
                                  cpf::type::nstr_t &&arg);

template <>
void write_arg<char *>(cpf::type::stream_t ustream,
                       cpf::type::str_t const &format, char *&&arg);

template <>
void write_arg<signed char *>(cpf::type::stream_t ustream,
                              cpf::type::str_t const &format,
                              signed char *&&arg);

template <>
void write_arg<const char *>(cpf::type::stream_t ustream,
                             cpf::type::str_t const &format, const char *&&arg);

template <>
void write_arg<const signed char *>(cpf::type::stream_t ustream,
                                    cpf::type::str_t const &format,
                                    const signed char *&&arg);

// TODO: add specialisation for "unsigned char" const and non const

// recursion-terminating function (counterpart to "update_ustream" with variadic
// arguments). This is the function executated when the API is called with only
// a format string and no arguments.
CPF_API void
update_ustream(cpf::type::stream_t ustream,
               const cpf::type::c_meta_iterator &end_point_comparator,
               cpf::type::c_meta_iterator &meta_iter,
               const cpf::type::str_t printed_string,
               const cpf::type::size search_start_pos);

// recursive call to process the format string as well as every argument
// provided. note: this function is not executed if no variadic arguments are
// respecified. using cfprintf_t and cprintf_t guarrantees the execution of this
// function.
template <typename T0, typename... Ts>
void update_ustream(cpf::type::stream_t ustream,
                    const cpf::type::c_meta_iterator &end_point_comparator,
                    cpf::type::c_meta_iterator &meta_iter,
                    const cpf::type::str_t printed_string,
                    const cpf::type::size search_start_pos, T0 &&arg0,
                    Ts &&... args) {
  cpf::type::str_t printed_string_ = printed_string;

  // printed string argument-specifier ('%') count
  const auto pstr_argc = cpf::intern::get_num_arg_specs(printed_string_);

  // more variadic args to write using "printed_string_" as format string.
  bool more_args_on_iter = false,
       // boolean used to signify whether variadic "arg0" has been passed to
      // std::fwprintf as an argument yet
      printed_arg0 = false;

  // string parsing start position...
  auto ssp_ = search_start_pos;

  if (pstr_argc >= 1) {
    auto arg_format_spec = cpf::intern::write_pre_arg_str(
        ustream, printed_string_, ssp_, meta_iter->second.first);

    // write the current argument i.e. "arg0" as formatted according to
    // "arg_format_spec" to the user stream
    cpf::intern::write_arg(ustream, arg_format_spec, std::forward<T0>(arg0));

    cpf::intern::write_post_arg_str(ustream, printed_string_, ssp_,
                                    more_args_on_iter, meta_iter,
                                    end_point_comparator);
    printed_arg0 = true;
  } else {
    cpf::intern::write_non_arg_str(ustream, printed_string_, ssp_, meta_iter);
  }

  bool iter_reached_end = (meta_iter == end_point_comparator);
  auto i_raw_str = iter_reached_end ? L"" : meta_iter->second.second;

  // only when "arg0" has been passed to std::fwprintf does variadic-argument
  // based recursion proceed onto the next one subsequently after arg0. Else
  // recurse back into this function with the same arguments.
  if (printed_arg0) {
    cpf::intern::update_ustream(
        ustream, end_point_comparator, meta_iter,
        (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
        more_args_on_iter, std::forward<Ts>(args)...);
  } else {
    cpf::intern::update_ustream(
        ustream, end_point_comparator, meta_iter,
        (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
        ssp_, std::forward<T0>(arg0), std::forward<Ts>(args)...);
  }
}

template <std::size_t FLAGS, typename T0, typename... Ts>
cpf::type::rcode_t dispatch(T0 &&f, Ts &&... args) {
  using namespace cpf::intern;
  using namespace cpf::type;

  rcode_t r(CPF_NO_ERR);
  stream_t s = ((FLAGS & CPF_STDO) == CPF_STDO) ? stdout : stderr;
  str_t f_;

  try {
    f_ = wconv(std::forward<T0>(f));
  }
  catch (...) {
    return rcode_t(CPF_NWCONV_ERR);
  }

  std::lock_guard<std::mutex> lock_(mtx_);
  {
    try {
#if CPF_DBG_CONFIG
      fmtspec_to_argtype_check(std::forward<const wchar_t *>(f_.c_str()),
                               std::forward<Ts>(args)...);
#endif
      save_stream_state(s);
      auto meta = process_format_string(f_);
      auto b = meta.cbegin();
      auto e = meta.cend();
      update_ustream(s, e, b, b->second.second, 0U, std::forward<Ts>(args)...);
    }
    catch (rcode_t &r_) {
      r = r_; // runtime error!
    }
    restore_stream_state(s, true);
  }

  return r;
}
}
}

#endif
