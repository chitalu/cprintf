#ifndef __CPF_HLPR_H__
#define __CPF_HLPR_H__

#include <algorithm>
#include <bitset>
#include <clocale>
#include <cwctype> //iswalpha
#include <memory>
#include <tuple>
#include <type_traits>

#include <mutex>

#ifdef CPF_WINDOWS_BUILD
// Note:	GCC does not yet support multi-byte conversion functionality
// from
// the following
// header, as a result narrow-string variants of cprintf's API will do nothing
// until
// this is resolved.

#include <codecvt> //wstring_convert
#endif
#include <cstdint>

#include <cprintf/internal/cpf_carg.h>
#include <cprintf/internal/cpf_scan.h>
#include <cprintf/internal/cpf_tconf.h>

#if CPF_DBG_CONFIG

#include <cstdarg>

#ifdef CPF_WINDOWS_BUILD
#define CPF_FILE_PATH_SEPARATOR (character == '\\' || character == '/');
#else
#define CPF_FILE_PATH_SEPARATOR (character == '/');
#endif

#define cprintf_dbg_impl_(format_, ...)                                        \
	do                                                                     \
	{                                                                      \
		const _cprintf_::unicode_string_t pname__ = CPF_WIDEN_STRING_LITERAL(__FILE__); \
		typedef struct                                                       \
		{                                                                    \
			bool operator()(char character) const                              \
			{                                                                  \
				return CPF_FILE_PATH_SEPARATOR                                   \
			}                                                                  \
		} sep__;                                                             \
		auto fname__ = _cprintf_::unicode_string_t(                                     \
		  std::find_if(pname__.rbegin(), pname__.rend(), sep__()).base(),    \
		  pname__.end());                                                    \
		cprintf_(stderr, _cprintf_::debugging_log_format_string, fname__, __TIME__,    \
		         __DATE__, __FUNCTION__, __LINE__);                          \
		cprintf(stderr, format_, ##__VA_ARGS__);                             \
	} while (false)

#endif

namespace _cprintf_
{

CPF_API const _cprintf_::unicode_string_t debugging_log_format_string;

CPF_API std::mutex mtx_;

// convert from narrow character string to wide character string @returns wide
// version of src
CPF_API unicode_string_t wconv(_cprintf_::ascii_string_t&& src);

// pass through ...
CPF_API unicode_string_t wconv(_cprintf_::unicode_string_t&& src);

// @returns number of printf argument tokens "%" in a given string
CPF_API std::int32_t get_num_arg_specs(const _cprintf_::unicode_string_t& unicode_string_t);

// print the substring preceding an argument specifier in a sub-format-string
CPF_API unicode_string_t write_substring_before_format_specifier(
  file_stream_t              file_stream,
  unicode_string_t&                printed_string_,
  std::int32_t&                 ssp_,
  const attribute_group_t attr);

// print the substring proceding an argument specifier in sub-format-string
CPF_API void write_substring_after_format_specifier(
  _cprintf_::file_stream_t                          file_stream,
  _cprintf_::unicode_string_t&                            printed_string_,
  std::int32_t&                             ssp_,
  bool&                                        more_args_on_iter,
  _cprintf_::format_string_layout_t::const_iterator&       meta_iter,
  const _cprintf_::format_string_layout_t::const_iterator& end_point_comparator);

// print non-argument specifying format string i.e where the implmentation need
// not invoke printf with any avariadic arguments.
CPF_API void write_substring_without_format_specifier(
  _cprintf_::file_stream_t                    file_stream,
  _cprintf_::unicode_string_t&                      printed_string_,
  std::int32_t&                       ssp_,
  _cprintf_::format_string_layout_t::const_iterator& meta_iter);

CPF_API _cprintf_::unicode_string_t resolve_str_frmt_spec(const _cprintf_::unicode_string_t& fs);

template <typename T>
void write_variadic_argument_to_file_stream(_cprintf_::file_stream_t     file_stream,
                                       _cprintf_::unicode_string_t const& format,
                                       T&&                     arg)
{
	const unicode_string_t f = resolve_str_frmt_spec(format);
	std::fwprintf(file_stream, f.c_str(), arg);
}

// extra wizardry has to be performed before printing the
// following types...
template <>
void write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
  _cprintf_::file_stream_t     file_stream,
  _cprintf_::unicode_string_t const& format,
  _cprintf_::unicode_string_t&&      arg);

template <>
void write_variadic_argument_to_file_stream<_cprintf_::ascii_string_t>(
  _cprintf_::file_stream_t     file_stream,
  _cprintf_::unicode_string_t const& format,
  _cprintf_::ascii_string_t&&     arg);

template <>
void write_variadic_argument_to_file_stream<char*>(_cprintf_::file_stream_t file_stream,
                                              _cprintf_::unicode_string_t const& format,
                                              char*&&                 arg);

template <>
void write_variadic_argument_to_file_stream<signed char*>(
  _cprintf_::file_stream_t     file_stream,
  _cprintf_::unicode_string_t const& format,
  signed char*&&          arg);

template <>
void write_variadic_argument_to_file_stream<const char*>(
  _cprintf_::file_stream_t     file_stream,
  _cprintf_::unicode_string_t const& format,
  const char*&&           arg);

template <>
void write_variadic_argument_to_file_stream<const signed char*>(
  _cprintf_::file_stream_t     file_stream,
  _cprintf_::unicode_string_t const& format,
  const signed char*&&    arg);

// recursion-terminating function (counterpart to "update_file_stream" with
// variadic
// arguments). This is the function executated when the API is called with only
// a format string and no arguments.
CPF_API void update_file_stream(
  _cprintf_::file_stream_t               file_stream,
  const _cprintf_::format_string_layout_const_iterator_t& end_point_comparator,
  _cprintf_::format_string_layout_const_iterator_t&       meta_iter,
  const _cprintf_::unicode_string_t            printed_string,
  const std::int32_t             search_start_pos);

// recursive call to process the format string as well as every argument
// provided. note: this function is not executed if no variadic arguments are
// respecified. using cfprintf_t and cprintf_t guarrantees the execution of this
// function.
template <typename T0, typename... Ts>
void update_file_stream(_cprintf_::file_stream_t               file_stream,
                        const _cprintf_::format_string_layout_const_iterator_t& end_point_comparator,
                        _cprintf_::format_string_layout_const_iterator_t&       meta_iter,
                        const _cprintf_::unicode_string_t            printed_string,
                        const std::int32_t             search_start_pos,
                        T0&&                              arg0,
                        Ts&&... args)
{
	_cprintf_::unicode_string_t printed_string_ = printed_string;

	// printed string argument-specifier ('%') count
	const auto pstr_argc = _cprintf_::get_num_arg_specs(printed_string_);

	// more variadic args to write using "printed_string_" as format string.
	bool more_args_on_iter = false,
	     // boolean used to signify whether variadic "arg0" has been passed to
	  // std::fwprintf as an argument yet
	  printed_arg0 = false;

	// string parsing start position...
	auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		auto arg_format_spec = _cprintf_::write_substring_before_format_specifier(
		  file_stream, printed_string_, ssp_, meta_iter->second.first);

		// write the current argument i.e. "arg0" as formatted according to
		// "arg_format_spec" to the user stream
		_cprintf_::write_variadic_argument_to_file_stream(file_stream, arg_format_spec,
		                                               std::forward<T0>(arg0));

		_cprintf_::write_substring_after_format_specifier(file_stream, printed_string_, ssp_,
		                                more_args_on_iter, meta_iter,
		                                end_point_comparator);
		printed_arg0 = true;
	}
	else
	{
		_cprintf_::write_substring_without_format_specifier(file_stream, printed_string_, ssp_,
		                               meta_iter);
	}

	bool iter_reached_end = (meta_iter == end_point_comparator);
	auto i_raw_str        = iter_reached_end ? L"" : meta_iter->second.second;

	// only when "arg0" has been passed to std::fwprintf does variadic-argument
	// based recursion proceed onto the next one subsequently after arg0. Else
	// recurse back into this function with the same arguments.
	if (printed_arg0)
	{
		_cprintf_::update_file_stream(
		  file_stream, end_point_comparator, meta_iter,
		  (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
		  more_args_on_iter, std::forward<Ts>(args)...);
	}
	else
	{
		_cprintf_::update_file_stream(
		  file_stream, end_point_comparator, meta_iter,
		  (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
		  ssp_, std::forward<T0>(arg0), std::forward<Ts>(args)...);
	}
}

template <typename T0, typename... Ts>
int dispatch(_cprintf_::file_stream_t stream, T0&& raw_format, Ts&&... args)
{
	_cprintf_::unicode_string_t format;
	try
	{
		format = _cprintf_::wconv(std::forward<T0>(raw_format));
	}
	catch (...)
	{
		// runtime string conversion error
		return CPF_NWCONV_ERR;
	}

	int error_code = CPF_NO_ERR;

	try
	{

#if CPF_DBG_CONFIG
		_cprintf_::format_specifier_correspondence_check(
		  std::forward<const wchar_t*>(format.c_str()), std::forward<Ts>(args)...);
#endif

		_cprintf_::save_stream_state(stream);

		auto meta_f   = _cprintf_::process_format_string(format);
		auto mf_begin = meta_f.cbegin();
		// end-point comparator...
		auto mf_endpoint_cmp = meta_f.cend();

		// make actual call to do printing and system terminal configurations
		_cprintf_::update_file_stream(stream, mf_endpoint_cmp, mf_begin,
		                                mf_begin->second.second, 0u,
		                                std::forward<Ts>(args)...);
	}
	catch (int thrown_error_code)
	{
		error_code = thrown_error_code; // runtime error!
	}

	_cprintf_::restore_stream_state(stream, true);

	return error_code;
}
}

#endif
