#include "cprintf/internal/cpf_srch.h"

#include "cprintf/cprintf.hpp"

#include <mutex>

namespace _cprintf_
{


#if CPF_DBG_CONFIG

	CPF_API const _cprintf_::unicode_string_t _cprintf_::debugging_log_format_string =
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

	std::mutex _cprintf_::mtx_;

	// cprintf("Characters:\t%c %%\n", 65);
	std::int32_t _cprintf_::get_num_arg_specs(const _cprintf_::unicode_string_t& obj)
	{
		std::int32_t n = 0u;
		std::uint32_t   pos = 0u;
		while ((pos = _cprintf_::search_for(L"%", obj, pos, '%')) < obj.size())
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

	_cprintf_::unicode_string_t _cprintf_::write_substring_before_format_specifier(
		_cprintf_::file_stream_t              file_stream,
		_cprintf_::unicode_string_t&                printed_string_,
		std::int32_t&                 ssp_,
		const _cprintf_::attribute_group_t attr)
	{
		_cprintf_::configure(file_stream, attr);

		ssp_ = _cprintf_::search_for(L"%", printed_string_, ssp_, '%');
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
		for (const auto& bfs : _cprintf_::std_fmt_specs)
		{
			if (printed_string_[ssp_ + 1] == bfs)
			{
				offset = 2;
				break;
			}
		}

		if (offset == 0)
		{
			_cprintf_::unicode_string_t specifier;
			bool             parsed_complete_f_spec = false;
			for (const auto& xfst : _cprintf_::ext_fmtspec_terms)
			{
				auto ps = printed_string_.substr(ssp_ + 1);

				auto _max = (ps.size());
				for (auto i(0u); i < _max; ++i)
				{
					auto crnt_char = ps[i];
					if (isalnum(crnt_char) ||
						is_in(crnt_char, _cprintf_::inter_fmt_specs) || crnt_char == xfst)
					{
						specifier.append({ crnt_char });

						if (crnt_char == xfst ||
							is_in(crnt_char, _cprintf_::ext_fmtspec_terms))
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
						!is_in(crnt_char, _cprintf_::ext_fmtspec_terms))
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

	void _cprintf_::write_substring_after_format_specifier(
		_cprintf_::file_stream_t                          file_stream,
		_cprintf_::unicode_string_t&                            printed_string_,
		std::int32_t&                             ssp_,
		bool&                                        more_args_on_iter,
		_cprintf_::format_string_layout_t::const_iterator&       meta_iter,
		const _cprintf_::format_string_layout_t::const_iterator& end_point_comparator)
	{
		printed_string_ = printed_string_.substr(ssp_);
		ssp_ = 0;

		more_args_on_iter = _cprintf_::get_num_arg_specs(printed_string_) > 0;
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
			std::advance(meta_iter, 1);
		}
	}

	void _cprintf_::write_substring_without_format_specifier(
		_cprintf_::file_stream_t                    file_stream,
		_cprintf_::unicode_string_t&                      printed_string_,
		std::int32_t&                       ssp_,
		_cprintf_::format_string_layout_t::const_iterator& meta_iter)
	{
		_cprintf_::configure(file_stream, meta_iter->second.first);

		ssp_ = 0;

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fwprintf(file_stream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
		std::advance(meta_iter, 1);

		while (_cprintf_::get_num_arg_specs(meta_iter->second.second) == 0)
		{
			_cprintf_::configure(file_stream, meta_iter->second.first);
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
			fwprintf(file_stream, meta_iter->second.second.c_str());
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

	_cprintf_::unicode_string_t _cprintf_::wconv(_cprintf_::ascii_string_t&& src)
	{
#ifdef CPF_LINUX_BUILD
		return _cprintf_::unicode_string_t(); // skip
#else
		std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
		return converter.from_bytes(src);
#endif
	}

	_cprintf_::unicode_string_t _cprintf_::wconv(_cprintf_::unicode_string_t&& src)
	{
		return std::move(src);
	}

	// printing strings with lower case 's' as format specifier leads
	// to undefined behaviour when using wide character strings .
	_cprintf_::unicode_string_t _cprintf_::resolve_str_frmt_spec(const _cprintf_::unicode_string_t& fs)
	{
		using namespace _cprintf_;

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
	void _cprintf_::write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
		_cprintf_::file_stream_t     file_stream,
		_cprintf_::unicode_string_t const& format,
		_cprintf_::unicode_string_t&&      arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream(file_stream, format,
			std::forward<const wchar_t*>(arg.c_str()));
	}

	template <>
	void _cprintf_::write_variadic_argument_to_file_stream<_cprintf_::ascii_string_t>(
		_cprintf_::file_stream_t     file_stream,
		_cprintf_::unicode_string_t const& format,
		_cprintf_::ascii_string_t&&     arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
			file_stream, format, std::forward<_cprintf_::unicode_string_t>(_cprintf_::wconv(
				std::forward<_cprintf_::ascii_string_t>(arg))));
	}

	template <>
	void _cprintf_::write_variadic_argument_to_file_stream<char*>(
		_cprintf_::file_stream_t file_stream, _cprintf_::unicode_string_t const& format, char*&& arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
			file_stream, format, std::forward<_cprintf_::unicode_string_t>(
				_cprintf_::wconv(std::forward<char*>(arg))));
	}

	template <>
	void _cprintf_::write_variadic_argument_to_file_stream<const char*>(
		_cprintf_::file_stream_t     file_stream,
		_cprintf_::unicode_string_t const& format,
		const char*&&           arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
			file_stream, format, std::forward<_cprintf_::unicode_string_t>(
				_cprintf_::wconv(std::forward<const char*>(arg))));
	}

	template <>
	void _cprintf_::write_variadic_argument_to_file_stream<signed char*>(
		_cprintf_::file_stream_t     file_stream,
		_cprintf_::unicode_string_t const& format,
		signed char*&&          arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
			file_stream, format, std::forward<_cprintf_::unicode_string_t>(
				_cprintf_::wconv(reinterpret_cast<char*>(arg))));
	}

	template <>
	void _cprintf_::write_variadic_argument_to_file_stream<const signed char*>(
		_cprintf_::file_stream_t     file_stream,
		_cprintf_::unicode_string_t const& format,
		const signed char*&&    arg)
	{
		using namespace _cprintf_;
		write_variadic_argument_to_file_stream<_cprintf_::unicode_string_t>(
			file_stream, format, std::forward<_cprintf_::unicode_string_t>(_cprintf_::wconv(
				reinterpret_cast<const char*>(arg))));
	}

	CPF_API void _cprintf_::update_file_stream(
		_cprintf_::file_stream_t                          file_stream,
		const _cprintf_::format_string_layout_t::const_iterator& end_point_comparator,
		_cprintf_::format_string_layout_t::const_iterator&       meta_iter,
		const _cprintf_::unicode_string_t                       printed_string = L"",
		const std::int32_t                        search_start_pos = 0)
	{
		while (meta_iter != end_point_comparator)
		{
			_cprintf_::configure(file_stream, meta_iter->second.first);

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

			fwprintf(file_stream, meta_iter->second.second.c_str());

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
			std::advance(meta_iter, 1);
		}

		/*restore defaults*/
		_cprintf_::configure(file_stream, _cprintf_::unicode_string_vector_t({ L"?" }));
	}

}