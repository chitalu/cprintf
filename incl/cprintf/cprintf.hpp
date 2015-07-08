//
//		Copyright(C)2014 Floyd Mulenga Chitalu
//
//    Permission is hereby granted, free of charge, to obtain a copy of this
//    software, to deal in the Software without restriction, including without
//    limitation the rights to[use], [copy], [modify], [merge], [publish],
//    [distribute], [sublicense], and / or [sell] copies of the Software, and to
//    permit persons to whom the Software is furnished to do so,subject to the
//    following conditions :
//
//        The above copyright notice and this permission notice shall be
//        included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//    AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S),
//    DAMAGE(S) OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __CPF_H__
#define __CPF_H__

#include <cprintf/internal/cpf_hlpr.h>

// core API
template <std::size_t FLAGS = CPF_STDO, typename F = cpf::type::str_t,
          typename... Fs>
std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
cprintf(F f, Fs... args) {
  using namespace cpf::type;
  using namespace cpf::intern;

  // TODO:
  // typedef typename std::result_of<decltype(cprintf<FLAGS, F, Fs...>(f,
  // args...))>::type rtype;
  typedef std::unique_ptr<status_t<verify_<FLAGS, F, Fs...>>> ret_t;

  ret_t r(new typename ret_t::element_type);
  r->f = f;

  CPF_MARK_CRITICAL_SECTION_;
  { r->c = dispatch<FLAGS>(std::forward<F>(f), std::forward<Fs>(args)...); }
  CPF_UNMARK_CRITICAL_SECTION_;

  return std::move(r);
}

// tuple API
template <std::size_t FLAGS = CPF_STDO, typename F, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
cprintf_t(F f, std::tuple<Fs...> args_tup) {
  using namespace cpf::intern;
  using namespace cpf::type;

  return std::move(apply_tuple(cprintf<FLAGS, F, Fs...>,
                               std::tuple_cat(std::make_tuple(f), args_tup)));
}

// string-literal API
template <std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
    cprintf_s(F(&f)[N], Fs... args) {
  using namespace cpf::intern;
  using namespace cpf::type;

  static_assert(
      N >= 2,
      "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");
  return cprintf<FLAGS>(f, std::forward<Fs>(args)...);
}

template <std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
cprintf_ts(F(&f)[N], std::tuple<Fs...> args_tup) {
  using namespace cpf::intern;
  using namespace cpf::type;

  static_assert(
      N >= 2,
      "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");
  return cprintf_t<FLAGS>(f, std::forward<std::tuple<Fs...>>(args_tup));
}

// X API
template <std::size_t FLAGS = CPF_STDO, typename X>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<
    FLAGS, cpf::type::str_t, typename cpf::type::xpromote_<X>::type>>>
cprintx(X x) {
  using namespace cpf::intern;
  using namespace cpf::type;

  typedef typename xpromote_<X>::type scalar_t;
  return std::move(x_impl<FLAGS, scalar_t>(
      std::forward<scalar_t>(static_cast<scalar_t>(x))));
}

#include <cprintf/internal/cpf_dbgh.h>

#endif /* __CPF_H__ */
