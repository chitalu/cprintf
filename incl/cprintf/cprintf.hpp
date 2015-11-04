#ifndef __CPF_H__
#define __CPF_H__

#include <cprintf/internal/cpf_hlpr.h>

// core API
template <std::size_t FLAGS = CPF_STDO, typename F = cpf::type::str_t,
          typename... Fs>
std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...> > >
cprintf(F f, Fs... args) {
  using namespace cpf::type;
  using namespace cpf::intern;

  typedef std::unique_ptr<status_t<verify_<FLAGS, F, Fs...> > > ret_t;
  ret_t r(new typename ret_t::element_type);
  r->f = f;
  r->c = dispatch<FLAGS>(std::forward<F>(f), std::forward<Fs>(args)...);
  return std::move(r);
}

// tuple API
template <std::size_t FLAGS = CPF_STDO, typename F, typename... Fs>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...> > >
cprintf_t(F f, std::tuple<Fs...> args_tup) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(apply_tuple(cprintf<FLAGS, F, Fs...>,
                               std::tuple_cat(std::make_tuple(f), args_tup)));
}

// string-literal API
template <std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...> > >
cprintf_s(F (&f)[N], Fs... args) {
  using namespace cpf::intern;
  using namespace cpf::type;

  static_assert(
      N >= 2,
      "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");
  return cprintf<FLAGS>(f, std::forward<Fs>(args)...);
}

template <std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...> > >
cprintf_ts(F (&f)[N], std::tuple<Fs...> arg) {
  using namespace cpf::intern;
  using namespace cpf::type;

  static_assert(
      N >= 2,
      "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");

  return cprintf_t<FLAGS>(f, std::forward<std::tuple<Fs...> >(arg));
}

// X API

template <std::size_t FLAGS = CPF_STDO, typename T>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str_t, T> > >
x_impl(
    typename std::enable_if<std::is_floating_point<T>::value, T>::type &&arg0) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(cprintf<FLAGS>(str_t(L"%f"), arg0));
}

template <std::size_t FLAGS = CPF_STDO, typename T>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str_t, T> > >
x_impl(typename std::enable_if<std::is_signed<T>::value, T>::type &&arg0) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(cprintf<FLAGS>(str_t(L"%lld"), arg0));
}

template <std::size_t FLAGS = CPF_STDO, typename T>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str_t, T> > >
x_impl(typename std::enable_if<std::is_unsigned<T>::value, T>::type &&arg0) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(cprintf<FLAGS>(str_t(L"%llu"), arg0));
}

template <std::size_t FLAGS = CPF_STDO, typename T>
inline std::unique_ptr<
    cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str_t, T> > >
x_impl(typename std::enable_if<std::is_pointer<T>::value, T>::type &&arg0) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(cprintf<FLAGS>(str_t(L"%p"), arg0));
}

template <std::size_t FLAGS = CPF_STDO, typename X>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<
    FLAGS, cpf::type::str_t, typename cpf::type::xpromote_<X>::type> > >
cprintx(X x) {
  using namespace cpf::intern;
  using namespace cpf::type;

  typedef typename xpromote_<X>::type scalar_t;
  return std::move(x_impl<FLAGS, scalar_t>(
      std::forward<scalar_t>(static_cast<scalar_t>(x))));
}

#include <cprintf/internal/cpf_dbgh.h>

#endif /* __CPF_H__ */
