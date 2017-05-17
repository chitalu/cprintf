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
		const unicode_string_t pname__ = CPF_WIDEN_STRING_LITERAL(__FILE__); \
		typedef struct                                                       \
		{                                                                    \
			bool operator()(char character) const                              \
			{                                                                  \
				return CPF_FILE_PATH_SEPARATOR                                   \
			}                                                                  \
		} sep__;                                                             \
		auto fname__ = unicode_string_t(                                     \
		  std::find_if(pname__.rbegin(), pname__.rend(), sep__()).base(),    \
		  pname__.end());                                                    \
		cprintf_(stderr, debugging_log_format_string, fname__, __TIME__,    \
		         __DATE__, __FUNCTION__, __LINE__);                          \
		cprintf(stderr, format_, ##__VA_ARGS__);                             \
	} while (false)

#endif

namespace _cprintf_
{

CPF_API const unicode_string_t debugging_log_format_string;

CPF_API std::mutex mtx_;

CPF_API unicode_string_t ascii_to_unicode_string_conversion(ascii_string_t&& src);

CPF_API unicode_string_t ascii_to_unicode_string_conversion(unicode_string_t&& src);

CPF_API std::int32_t get_num_format_specifiers_in_string(const unicode_string_t& unicode_string_t);

CPF_API unicode_string_t write_substring_before_format_specifier(
  file_stream_t              file_stream,
  unicode_string_t&                printed_string_,
  std::int32_t&                 ssp_,
  const attribute_group_t attr);

CPF_API void write_substring_after_format_specifier(
  file_stream_t                          file_stream,
  unicode_string_t&                            printed_string_,
  std::int32_t&                             ssp_,
  bool&                                        more_args_on_iter,
  format_string_layout_t::const_iterator&       format_string_layout_iterator,
  const format_string_layout_t::const_iterator& end_point_comparator);

CPF_API void write_substring_without_format_specifier(
  file_stream_t                    file_stream,
  unicode_string_t&                      printed_string_,
  std::int32_t&                       ssp_,
  format_string_layout_t::const_iterator& format_string_layout_iterator);

CPF_API unicode_string_t resolve_string_type_format_specifier(const unicode_string_t& fs);

template <typename T>
void write_variadic_argument_to_file_stream(file_stream_t     file_stream,
                                       unicode_string_t const& format,
                                       T&&                     arg)
{
	const unicode_string_t f = resolve_string_type_format_specifier(format);
	std::fwprintf(file_stream, f.c_str(), arg);
}

template <>
void write_variadic_argument_to_file_stream<unicode_string_t>(
  file_stream_t     file_stream,
  unicode_string_t const& format,
  unicode_string_t&&      arg);

template <>
void write_variadic_argument_to_file_stream<ascii_string_t>(
  file_stream_t     file_stream,
  unicode_string_t const& format,
  ascii_string_t&&     arg);

template <>
void write_variadic_argument_to_file_stream<char*>(file_stream_t file_stream,
                                              unicode_string_t const& format,
                                              char*&&                 arg);

template <>
void write_variadic_argument_to_file_stream<signed char*>(
  file_stream_t     file_stream,
  unicode_string_t const& format,
  signed char*&&          arg);

template <>
void write_variadic_argument_to_file_stream<const char*>(
  file_stream_t     file_stream,
  unicode_string_t const& format,
  const char*&&           arg);

template <>
void write_variadic_argument_to_file_stream<const signed char*>(
  file_stream_t     file_stream,
  unicode_string_t const& format,
  const signed char*&&    arg);

CPF_API void print_format_string_layout(
  file_stream_t               file_stream,
  const format_string_layout_const_iterator_t& end_point_comparator,
  format_string_layout_const_iterator_t&       format_string_layout_iterator,
  const unicode_string_t            printed_string,
  const std::int32_t             search_start_pos);

template <typename ArgType0, typename... RemainingArgTypes>
void print_format_string_layout(file_stream_t               file_stream,
                        const format_string_layout_const_iterator_t& end_point_comparator,
                        format_string_layout_const_iterator_t&       format_string_layout_iterator,
                        const unicode_string_t            printed_string,
                        const std::int32_t             search_start_pos,
	ArgType0&&                              arg0,
	RemainingArgTypes&&... args)
{
	// printed string argument-specifier ('%') count
	const auto pstr_argc = get_num_format_specifiers_in_string(printed_string);

	// more variadic args to write using "printed_string_" as format string.
	bool more_args_on_iter = false,
	     // boolean used to signify whether variadic "arg0" has been passed to
	  // std::fwprintf as an argument yet
	  printed_arg0 = false;

	// string parsing start position...
	auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		const unicode_string_t argument_format_specifier = write_substring_before_format_specifier(
		  file_stream, printed_string_, ssp_, format_string_layout_iterator->second.first);

		write_variadic_argument_to_file_stream(file_stream, argument_format_specifier, std::forward<ArgType0>(arg0));

		write_substring_after_format_specifier(file_stream, printed_string_, ssp_,
		                                more_args_on_iter, format_string_layout_iterator,
		                                end_point_comparator);
		printed_arg0 = true;
	}
	else
	{
		write_substring_without_format_specifier(file_stream, printed_string_, ssp_,
		                               format_string_layout_iterator);
	}

	const bool iter_reached_end = (format_string_layout_iterator == end_point_comparator);
	const unicode_string_t i_raw_str = iter_reached_end ? L"" : format_string_layout_iterator->second.second;

	// only when "arg0" has been passed to std::fwprintf does variadic-argument
	// based recursion proceed onto the next one subsequently after arg0. Else
	// recurse back into this function with the same arguments.
	if (printed_arg0)
	{
		print_format_string_layout(
		  file_stream, end_point_comparator, format_string_layout_iterator,
		  (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
		  more_args_on_iter, std::forward<Ts>(args)...);
	}
	else
	{
		print_format_string_layout(
		  file_stream, end_point_comparator, format_string_layout_iterator,
		  (!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
		  ssp_, std::forward<ArgType0>(arg0), std::forward<RemainingArgTypes>(args)...);
	}
}

template <typename FormatType, typename... ArgumentTypes>
int dispatch(file_stream_t file_stream, FormatType&& raw_format, ArgumentTypes&&... args)
{
	unicode_string_t format;

	try
	{
		format = ascii_to_unicode_string_conversion(std::forward<T0>(raw_format));
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
		format_specifier_correspondence_check( std::forward<const wchar_t*>(format.c_str()), std::forward<Ts>(args)...);
#endif

		save_stream_state(file_stream);

		format_string_layout_t format_string_layout   = parse_format_string(format);
		format_string_layout_const_iterator_t format_string_layout_begin_iterator = format_string_layout.cbegin();
		format_string_layout_const_iterator_t format_string_layout_end_iterator = format_string_layout.cend();

		std::lock_guard<std::mutex> lock(_cprintf_::mtx_);

		print_format_string_layout(file_stream, format_string_layout_end_iterator, format_string_layout_begin_iterator,
		                                format_string_layout_begin_iterator->second.second, 0u,
		                                std::forward<ArgumentTypes>(args)...);
	}
	catch (int thrown_error_code)
	{
		error_code = thrown_error_code;
	}

	restore_stream_state(file_stream, true);

	return error_code;
}
}

#endif
