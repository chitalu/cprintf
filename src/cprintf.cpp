#include "cprintf/internal/cpf_srch.h"

#include "cprintf/cprintf.hpp"

#include <mutex>

namespace _cprintf_
{


#if CPF_DBG_CONFIG

	CPF_API const unicode_string_t debugging_log_format_string =
#ifndef CPF_WINDOWS_BUILD
		LR"debug_str($cdbg
$g@file:$c	$g*%S$c
$g@time:$c	$g*%S$c-$g*%S$c 
$g@func:$c	$g*%S$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#else  // then use small 's' for string args on windows
		LR"debug_str($cdbg
$g@file:$c	$g*%s$c
$g@time:$c	$g*%s$c-$g*%s$c 
$g@func:$c	$g*%s$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#endif // !CPF_LINUX_BUILD

#endif // CPF_DBG_CONFIG

	std::mutex mtx_;

	// cprintf("Characters:\t%c %%\n", 65);
	std::int32_t get_num_format_specifiers_in_string(const unicode_string_t& obj)
	{
		std::int32_t n = 0u;
		std::uint32_t   pos = 0u;
		while ((pos = search_for(L"%", obj, pos, '%')) < obj.size())
		{
			if (pos == obj.size() - 1)
			{
				/*this would imply the following: cprintf("foo bar %");*/
				throw CPF_FSPEC_ERR; // invalid format specifier('%') position.
			}
			std::int32_t n_ = n;

			/*entering the while loop implies that a '%' was found successfully
			which means we check whether the proceeding character is a '%'
			if its not, then we are not attempting to escape the % character
			using standard printf i.e %%*/
			if (obj[(pos + 1)] != '%')
			{
				/*implies a case when you have %%%_ where "_" is a format specifier such
				 * as d -> %%%d*/
				std::int32_t p_2 = (pos - 2);
				if (p_2 >= 0)
				{
					if (obj[(pos - 2)] == '%')
					{
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

	unicode_string_t write_substring_before_format_specifier(
		file_stream_t              file_stream,
		unicode_string_t&                printed_string_,
		std::int32_t&                 ssp_,
		const attribute_group_t attr)
	{
		configure(file_stream, attr);

		ssp_ = search_for(L"%", printed_string_, ssp_, '%');
		if (ssp_ != 0)
		{
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
			fwprintf(file_stream, printed_string_.substr(0, ssp_).c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
		}

		auto is_in = [=](const wchar_t&                        _target,
			const std::initializer_list<wchar_t>& _list) -> bool {
			for (auto& i : _list)
			{
				if (i == _target)
				{
					return true;
				}
			}
			return false;
		};

		/* format specifiers beging with % and end with a space*/
		auto            space_pos = printed_string_.find(' ', ssp_);
		std::int32_t offset = 0;
		for (const auto& bfs : std_fmt_specs)
		{
			if (printed_string_[ssp_ + 1] == bfs)
			{
				offset = 2;
				break;
			}
		}

		if (offset == 0)
		{
			unicode_string_t specifier;
			bool             parsed_complete_f_spec = false;
			for (const auto& xfst : ext_fmtspec_terms)
			{
				auto ps = printed_string_.substr(ssp_ + 1);

				auto _max = (ps.size());
				for (auto i(0u); i < _max; ++i)
				{
					auto crnt_char = ps[i];
					if (isalnum(crnt_char) ||
						is_in(crnt_char, inter_fmt_specs) || crnt_char == xfst)
					{
						specifier.append({ crnt_char });

						if (crnt_char == xfst ||
							is_in(crnt_char, ext_fmtspec_terms))
						{
							parsed_complete_f_spec = true;
							break;
						}
					}
					else
					{
						throw CPF_FSPEC_ERR; // invalid format specifier detail
					}

					// last iteration
					if (i == (_max - 1) &&
						!is_in(crnt_char, ext_fmtspec_terms))
					{
						throw CPF_FSPEC_ERR; // invalid format specifier
					}
				}
				if (parsed_complete_f_spec)
				{
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

	void write_substring_after_format_specifier(
		file_stream_t                          file_stream,
		unicode_string_t&                            printed_string_,
		std::int32_t&                             ssp_,
		bool&                                        more_args_on_iter,
		format_string_layout_t::const_iterator&       format_string_layout_iterator,
		const format_string_layout_t::const_iterator& end_point_comparator)
	{
		printed_string_ = printed_string_.substr(ssp_);
		ssp_ = 0;

		more_args_on_iter = get_num_format_specifiers_in_string(printed_string_) > 0;
		if (!more_args_on_iter)
		{
			if (!printed_string_.empty())
			{
#ifdef CPF_LINUX_BUILD
				/*see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
				fwprintf(file_stream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
				printed_string_.clear();
			}
			std::advance(format_string_layout_iterator, 1);
		}
	}

	void write_substring_without_format_specifier(
		file_stream_t                    file_stream,
		unicode_string_t&                      printed_string_,
		std::int32_t&                       ssp_,
		format_string_layout_t::const_iterator& format_string_layout_iterator)
	{
		configure(file_stream, format_string_layout_iterator->second.first);

		ssp_ = 0;

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fwprintf(file_stream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
		std::advance(format_string_layout_iterator, 1);

		while (get_num_format_specifiers_in_string(format_string_layout_iterator->second.second) == 0)
		{
			configure(file_stream, format_string_layout_iterator->second.first);
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
			fwprintf(file_stream, format_string_layout_iterator->second.second.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
			std::advance(format_string_layout_iterator, 1);
		}
	}

	unicode_string_t ascii_to_unicode_string_conversion(ascii_string_t&& mbstr)
	{
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
		const char* _mbstr = &mbstr[0];
		// Converts a null-terminated multibyte character sequence, which begins in
		// the conversion state
		std::int32_t len = 1 + std::mbsrtowcs(NULL, &_mbstr, 0, &state);
		unicode_string_t wstr(len, 0);
		std::mbsrtowcs(&wstr[0], &_mbstr, wstr.size(), &state);
		return wstr;
	}

	unicode_string_t ascii_to_unicode_string_conversion(unicode_string_t&& src)
	{
		return std::move(src);
	}

	// printing strings with lower case 's' as format specifier leads
	// to undefined behaviour when using wide character strings .
	unicode_string_t resolve_string_type_format_specifier(const unicode_string_t& fs)
	{
		unicode_string_t f = fs;
		// seems like msvc does not conform to wide character
		// format specifier rules
#ifndef CPF_WINDOWS_BUILD
		if (fs.find('s') != unicode_string_t::npos)
		{
			f.clear();
			for (std::wint_t i(0); i < fs.size(); ++i)
			{
				f.append(
				{ std::iswalpha(fs[i]) ? (wchar_t)std::towupper(fs[i]) : fs[i] });
			}
		}
#endif
		return f;
	}

	template <>
	void write_variadic_argument_to_file_stream<unicode_string_t>(
		file_stream_t     file_stream,
		unicode_string_t const& format,
		unicode_string_t&&      arg)
	{
		write_variadic_argument_to_file_stream(file_stream, format,
			std::forward<const wchar_t*>(arg.c_str()));
	}

	template <>
	void write_variadic_argument_to_file_stream<ascii_string_t>(
		file_stream_t     file_stream,
		unicode_string_t const& format,
		ascii_string_t&&     arg)
	{
		write_variadic_argument_to_file_stream<unicode_string_t>(
			file_stream, format, std::forward<unicode_string_t>(ascii_to_unicode_string_conversion(
				std::forward<ascii_string_t>(arg))));
	}

	template <>
	void write_variadic_argument_to_file_stream<char*>(
		file_stream_t file_stream, unicode_string_t const& format, char*&& arg)
	{
		write_variadic_argument_to_file_stream<unicode_string_t>(
			file_stream, format, std::forward<unicode_string_t>(
				ascii_to_unicode_string_conversion(std::forward<char*>(arg))));
	}

	template <>
	void write_variadic_argument_to_file_stream<const char*>(
		file_stream_t     file_stream,
		unicode_string_t const& format,
		const char*&&           arg)
	{
		write_variadic_argument_to_file_stream<unicode_string_t>(
			file_stream, format, std::forward<unicode_string_t>(
				ascii_to_unicode_string_conversion(std::forward<const char*>(arg))));
	}

	template <>
	void write_variadic_argument_to_file_stream<signed char*>(
		file_stream_t     file_stream,
		unicode_string_t const& format,
		signed char*&&          arg)
	{
		write_variadic_argument_to_file_stream<unicode_string_t>(
			file_stream, format, std::forward<unicode_string_t>(
				ascii_to_unicode_string_conversion(reinterpret_cast<char*>(arg))));
	}

	template <>
	void write_variadic_argument_to_file_stream<const signed char*>(
		file_stream_t     file_stream,
		unicode_string_t const& format,
		const signed char*&&    arg)
	{
		write_variadic_argument_to_file_stream<unicode_string_t>(
			file_stream, format, std::forward<unicode_string_t>(ascii_to_unicode_string_conversion(
				reinterpret_cast<const char*>(arg))));
	}

	CPF_API void print_format_string_layout(
		file_stream_t                          file_stream,
		const format_string_layout_t::const_iterator& end_point_comparator,
		format_string_layout_t::const_iterator&       format_string_layout_iterator,
		const unicode_string_t                       printed_string = L"",
		const std::int32_t                        search_start_pos = 0)
	{
		while (format_string_layout_iterator != end_point_comparator)
		{
			configure(file_stream, format_string_layout_iterator->second.first);

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

			fwprintf(file_stream, format_string_layout_iterator->second.second.c_str());

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
			std::advance(format_string_layout_iterator, 1);
		}

		/*restore defaults*/
		configure(file_stream, unicode_string_vector_t({ L"?" }));
	}

}