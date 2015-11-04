#include "cprintf/internal/cpf_srch.h"

#include "cprintf/cprintf.hpp"

#include <mutex>

#if CPF_DBG_CONFIG
/*
	wide character string debug log
*/
CPF_API const cpf::type::str_t cpf::intern::dbg_log_fmt_str =
#if !CPF_WINDOWS_BUILD
LR"debug_str($cdbg
$g@file:$c	$g*%S$c
$g@time:$c	$g*%S$c-$g*%S$c 
$g@func:$c	$g*%S$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#else // then use small 's' for string args on windows
LR"debug_str($cdbg
$g@file:$c	$g*%s$c
$g@time:$c	$g*%s$c-$g*%s$c 
$g@func:$c	$g*%s$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#endif// !CPF_LINUX_BUILD

#endif // CPF_DBG_CONFIG

std::mutex cpf::intern::mtx_;

// cprintf("Characters:\t%c %%\n", 65);
cpf::type::size cpf::intern::get_num_arg_specs(const cpf::type::str_t &obj) {
  cpf::type::size n = 0u;
  std::uint32_t pos = 0u;
  while ((pos = cpf::intern::search_for(L"%", obj, pos, '%')) < obj.size()) {
    if (pos == obj.size() - 1) {
      /*this would imply the following: cprintf("foo bar %");*/
      throw CPF_FSPEC_ERR; // invalid format specifier('%') position.
    }
    std::int32_t n_ = n;

    /*entering the while loop implies that a '%' was found successfully
    which means we check whether the proceeding character is a '%'
    if its not, then we are not attempting to escape the % character
    using standard printf i.e %%*/
    if (obj[(pos + 1)] != '%') {
      /*implies a case when you have %%%_ where "_" is a format specifier such
       * as d -> %%%d*/
      std::int32_t p_2 = (pos - 2);
      if (p_2 >= 0) {
        if (obj[(pos - 2)] == '%') {
          n_ = ++n;
        }
      }

      if (n_ == n)
        ++n;
    }
    ++pos;
  }
  return n;
}

cpf::type::str_t cpf::intern::write_pre_arg_str(
    cpf::type::stream_t ustream, cpf::type::str_t &printed_string_,
    cpf::type::size &ssp_, const cpf::type::attribute_group attr) {
  cpf::intern::configure(ustream, attr);

  ssp_ = cpf::intern::search_for(L"%", printed_string_, ssp_, '%');
  if (ssp_ != 0) {
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
    fwprintf(ustream, printed_string_.substr(0, ssp_).c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
  }

  auto is_in = [=](const wchar_t & _target,
                   const std::initializer_list<wchar_t> & _list)->bool {
    for (auto &i : _list) {
      if (i == _target) {
        return true;
      }
    }
    return false;
  };

  /* format specifiers beging with % and end with a space*/
  auto space_pos = printed_string_.find(' ', ssp_);
  cpf::type::size offset = 0;
  for (const auto &bfs : cpf::intern::std_fmt_specs) {
    if (printed_string_[ssp_ + 1] == bfs) {
      offset = 2;
      break;
    }
  }

  if (offset == 0) {
    cpf::type::str_t specifier;
    bool parsed_complete_f_spec = false;
    for (const auto &xfst : cpf::intern::ext_fmtspec_terms) {
      auto ps = printed_string_.substr(ssp_ + 1);

      auto _max = (ps.size());
      for (auto i(0u); i < _max; ++i) {
        auto crnt_char = ps[i];
        if (isalnum(crnt_char) ||
            is_in(crnt_char, cpf::intern::inter_fmt_specs) ||
            crnt_char == xfst) {
          specifier.append({ crnt_char });

          if (crnt_char == xfst ||
              is_in(crnt_char, cpf::intern::ext_fmtspec_terms)) {
            parsed_complete_f_spec = true;
            break;
          }
        } else {
          throw CPF_FSPEC_ERR; // invalid format specifier detail
        }

        // last iteration
        if (i == (_max - 1) &&
            !is_in(crnt_char, cpf::intern::ext_fmtspec_terms)) {
          throw CPF_FSPEC_ERR; // invalid format specifier
        }
      }
      if (parsed_complete_f_spec) {
        break;
      }

      specifier.clear();
    }

    offset = specifier.size() + 1;
  }

  auto fstr = printed_string_.substr(ssp_, offset);
  ssp_ += offset;
  return fstr;
}

void cpf::intern::write_post_arg_str(
    cpf::type::stream_t ustream, cpf::type::str_t &printed_string_,
    cpf::type::size &ssp_, bool &more_args_on_iter,
    cpf::type::meta_fmt_t::const_iterator &meta_iter,
    const cpf::type::meta_fmt_t::const_iterator &end_point_comparator) {
  printed_string_ = printed_string_.substr(ssp_);
  ssp_ = 0;

  more_args_on_iter = cpf::intern::get_num_arg_specs(printed_string_) > 0;
  if (!more_args_on_iter) {
    if (!printed_string_.empty()) {
#ifdef CPF_LINUX_BUILD
/*see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
      fwprintf(ustream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
      printed_string_.clear();
    }
    std::advance(meta_iter, 1);
  }
}

void cpf::intern::write_non_arg_str(
    cpf::type::stream_t ustream, cpf::type::str_t &printed_string_,
    cpf::type::size &ssp_, cpf::type::meta_fmt_t::const_iterator &meta_iter) {
  cpf::intern::configure(ustream, meta_iter->second.first);

  ssp_ = 0;

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
  fwprintf(ustream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
  std::advance(meta_iter, 1);

  while (cpf::intern::get_num_arg_specs(meta_iter->second.second) == 0) {
    cpf::intern::configure(ustream, meta_iter->second.first);
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
    fwprintf(ustream, meta_iter->second.second.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
    std::advance(meta_iter, 1);
  }
}

/*
        in the case of possible future issues, it may be desirable to use
        std::codecvt_utf8_utf16<wchar_t> instead of std::codecvt_utf8<wchar_t>

        perhaps it could even be possible to add a printf function variant that
        can be instantiated with either
*/
#include <clocale>
#include <cwchar>
cpf::type::str_t cpf::intern::wconv(cpf::type::nstr_t &&mbstr) {
  /*
   The mbstate_t is a trivial non-array type that can represent any of the
conversion states that can occur in an implementation-defined set of supported
multibyte character encoding rules. Zero-initialized value of mbstate_t
represents the initial conversion state, although other values of mbstate_t may
exist that also represent the initial conversion state.

Possible implementation of mbstate_t is a struct type holding an array
representing the incomplete multibyte character, an integer counter indicating
the number of bytes in the array that have been processed, and a representation
of the current shift state. type mbstate_t is a trivial non-array type that can
represent any of the conversion states that can occur in an
implementation-defined set of supported multibyte character encoding rules.
Zero-initialized value of mbstate_t represents the initial conversion state,
although other values of mbstate_t may exist that also represent the initial
conversion state.
 */
  std::mbstate_t state = std::mbstate_t();
  const char* _mbstr= &mbstr[0];
  // Converts a null-terminated multibyte character sequence, which begins in
  // the conversion state
  std::int32_t len = 1 + std::mbsrtowcs(NULL, &_mbstr, 0, &state);
  cpf::type::str_t wstr(len, 0);
  std::mbsrtowcs(&wstr[0], &_mbstr, wstr.size(), &state);
  return wstr;
}

cpf::type::str_t cpf::intern::wconv(cpf::type::str_t &&src) {
  return std::move(src);
}

// printing strings with lower case 's' as format specifier leads
// to undefined behaviour when using wide character strings .
cpf::type::str_t
cpf::intern::resolve_str_frmt_spec(const cpf::type::str_t &fs) {
  using namespace cpf::type;

  str_t f = fs;
// seems like msvc does not conform to wide character
// format specifier rules
#if !CPF_WINDOWS_BUILD
  if (fs.find('s') != str_t::npos) {
    f.clear();
    for (std::wint_t i(0); i < fs.size(); ++i) {
      f.append(
          { std::iswalpha(fs[i]) ? (wchar_t)std::towupper(fs[i]) : fs[i] });
    }
  }
#endif
  return f;
}

template <>
void cpf::intern::write_arg<cpf::type::str_t>(cpf::type::stream_t ustream,
                                              cpf::type::str_t const &format,
                                              cpf::type::str_t &&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg(ustream, format, std::forward<const wchar_t *>(arg.c_str()));
}

template <>
void cpf::intern::write_arg<cpf::type::nstr_t>(cpf::type::stream_t ustream,
                                               cpf::type::str_t const &format,
                                               cpf::type::nstr_t &&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg<cpf::type::str_t>(ustream, format,
                              std::forward<cpf::type::str_t>(cpf::intern::wconv(
                                  std::forward<cpf::type::nstr_t>(arg))));
}

template <>
void cpf::intern::write_arg<char *>(cpf::type::stream_t ustream,
                                    cpf::type::str_t const &format,
                                    char *&&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg<cpf::type::str_t>(
      ustream, format, std::forward<cpf::type::str_t>(
                           cpf::intern::wconv(std::forward<char *>(arg))));
}

template <>
void cpf::intern::write_arg<const char *>(cpf::type::stream_t ustream,
                                          cpf::type::str_t const &format,
                                          const char *&&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg<cpf::type::str_t>(ustream, format,
                              std::forward<cpf::type::str_t>(cpf::intern::wconv(
                                  std::forward<const char *>(arg))));
}

template <>
void cpf::intern::write_arg<signed char *>(cpf::type::stream_t ustream,
                                           cpf::type::str_t const &format,
                                           signed char *&&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg<cpf::type::str_t>(
      ustream, format, std::forward<cpf::type::str_t>(
                           cpf::intern::wconv(reinterpret_cast<char *>(arg))));
}

template <>
void cpf::intern::write_arg<const signed char *>(cpf::type::stream_t ustream,
                                                 cpf::type::str_t const &format,
                                                 const signed char *&&arg) {
  using namespace cpf::intern;
  using namespace cpf::type;
  write_arg<cpf::type::str_t>(ustream, format,
                              std::forward<cpf::type::str_t>(cpf::intern::wconv(
                                  reinterpret_cast<const char *>(arg))));
}

CPF_API void cpf::intern::update_ustream(
    cpf::type::stream_t ustream,
    const cpf::type::meta_fmt_t::const_iterator &end_point_comparator,
    cpf::type::meta_fmt_t::const_iterator &meta_iter,
    const cpf::type::str_t printed_string = L"",
    const cpf::type::size search_start_pos = 0) {
  while (meta_iter != end_point_comparator) {
    cpf::intern::configure(ustream, meta_iter->second.first);

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

    fwprintf(ustream, meta_iter->second.second.c_str());

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
    std::advance(meta_iter, 1);
  }

  /*restore defaults*/
  cpf::intern::configure(ustream, cpf::type::str_vec_t({ L"?" }));
}
