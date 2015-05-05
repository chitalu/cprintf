/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

*/

#include "cprintf/internal/cpf_srch.h"

#include "cprintf/cprintf.hpp"

#include <mutex>

#if CPF_DBG_CONFIG
/*
	wide character string debug log
*/
CPF_API const cpf::type::str_t cpf::intern::dbg_log_fmt_str =
LR"debug_str($cdbg
$g@file:$c	$g*%s$c
$g@time:$c	$g*%s$c-$g*%s$c 
$g@func:$c	$g*%s$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#endif //CPF_DBG_CONFIG

/*
	atomicity mutex variable used for synchronising client-multithreaded invocations of
	API
*/
std::mutex cpf::intern::user_thread_mutex;

//cprintf("Characters:\t%c %%\n", 65);
cpf::type::size cpf::intern::get_num_arg_specs(const cpf::type::str_t & obj)
{
	cpf::type::size n = 0u;
	std::uint32_t pos = 0u;
	while ((pos = cpf::intern::search_for(L"%", obj, pos, '%')) < obj.size())
	{
		if (pos == obj.size() - 1)
		{
			/*this would imply the following: cprintf("foo bar %");*/
			throw CPF_FSPEC_ERR;// invalid format specifier('%') position.
		}
		std::int32_t n_ = n;

		/*entering the while loop implies that a '%' was found successfully
		which means we check whether the proceeding character is a '%'
		if its not, then we are not attempting to escape the % character
		using standard printf i.e %%*/
		if (obj[(pos + 1)] != '%')
		{
			/*implies a case when you have %%%_ where "_" is a format specifier such as d -> %%%d*/
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

cpf::type::str_t cpf::intern::write_pre_arg_str(	cpf::type::stream ustream,
												cpf::type::str_t& printed_string_,
												cpf::type::size& ssp_,
												const cpf::type::attribute_group attr)
{
	cpf::intern::configure(ustream, attr);

	ssp_ = cpf::intern::search_for(L"%", printed_string_, ssp_, '%');
	if (ssp_ != 0)
	{
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fwprintf(ustream, printed_string_.substr(0, ssp_).c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
	}

	auto is_in = [=](const wchar_t& _target, const std::initializer_list<wchar_t>& _list)->bool
	{
		for (auto &i : _list)
		{
			if (i == _target)
			{
				return true;
			}
		}
		return false;
	};

	/* format specifiers beging with % and end with a space*/
	auto space_pos = printed_string_.find(' ', ssp_);
	cpf::type::size offset = 0;
	for (const auto &bfs : cpf::intern::std_fmt_specs)
	{
		if (printed_string_[ssp_ + 1] == bfs)
		{
			offset = 2;
			break;
		}
	}

	if (offset == 0)
	{
		cpf::type::str_t specifier;
		bool parsed_complete_f_spec = false;
		for (const auto &xfst : cpf::intern::ext_fmtspec_terms)
		{
			auto ps = printed_string_.substr(ssp_ + 1);
	
			auto _max = (ps.size());
			for (auto i(0u); i < _max; ++i)
			{
				auto crnt_char = ps[i];
				if (isalnum(crnt_char) || is_in(crnt_char, cpf::intern::inter_fmt_specs) || crnt_char == xfst)
				{
					specifier.append({ crnt_char });

					if (crnt_char == xfst || is_in(crnt_char, cpf::intern::ext_fmtspec_terms))
					{
						parsed_complete_f_spec = true;
						break;
					}
				}
				else
				{
					throw CPF_FSPEC_ERR;// invalid format specifier detail
				}

				//last iteration
				if (i == (_max - 1) && !is_in(crnt_char, cpf::intern::ext_fmtspec_terms))
				{
					throw CPF_FSPEC_ERR;// invalid format specifier
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

void cpf::intern::write_post_arg_str(	cpf::type::stream ustream,
										cpf::type::str_t& printed_string_,
										cpf::type::size& ssp_,
										bool &more_args_on_iter,
										cpf::type::meta_fmt_t::const_iterator &meta_iter,
										const cpf::type::meta_fmt_t::const_iterator &end_point_comparator)
{
	printed_string_ = printed_string_.substr(ssp_);
	ssp_ = 0;

	more_args_on_iter = cpf::intern::get_num_arg_specs(printed_string_) > 0;
	if (!more_args_on_iter)
	{
		if (!printed_string_.empty())
		{
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

void cpf::intern::write_non_arg_str(cpf::type::stream ustream,
									cpf::type::str_t& printed_string_,
									cpf::type::size& ssp_,
									cpf::type::meta_fmt_t::const_iterator &meta_iter)
{
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

	while (cpf::intern::get_num_arg_specs(meta_iter->second.second) == 0)
	{
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

cpf::type::str_t cpf::intern::wconv(cpf::type::nstr_t &&src)
{
#ifdef CPF_LINUX_BUILD
	return cpf::type::str_t(); //skip
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(src);
#endif
}

cpf::type::str_t cpf::intern::wconv(cpf::type::str_t &&src)
{
	return std::move(src);
}

template<>
void cpf::intern::write_arg<cpf::type::str_t>(cpf::type::stream ustream,
											cpf::type::str_t const &format,
											cpf::type::str_t&& arg)
{
	cpf::intern::write_arg(ustream, format, std::forward<const wchar_t*>(arg.c_str()));
}

template<>
void cpf::intern::write_arg<cpf::type::nstr_t>(cpf::type::stream ustream,
												cpf::type::str_t const &format,
												cpf::type::nstr_t&& arg)
{
	cpf::intern::write_arg<cpf::type::str_t>(ustream, format, std::forward<cpf::type::str_t>(cpf::intern::wconv(std::forward<cpf::type::nstr_t>(arg))));
}

template<>
void cpf::intern::write_arg<char*>(cpf::type::stream ustream,
									cpf::type::str_t const &format,
									char*&& arg)
{
	cpf::intern::write_arg<cpf::type::str_t>(ustream, format, std::forward<cpf::type::str_t>(cpf::intern::wconv(std::forward<char*>(arg))));
}

template<>
void cpf::intern::write_arg<const char*>(cpf::type::stream ustream,
											cpf::type::str_t const &format,
											const char*&& arg)
{
	cpf::intern::write_arg<cpf::type::str_t>(	ustream, format, std::forward<cpf::type::str_t>(cpf::intern::wconv(std::forward<const char*>(arg))));
}

template<>
void cpf::intern::write_arg<signed char*>(cpf::type::stream ustream,
											cpf::type::str_t const &format,
											signed char*&& arg)
{
	cpf::intern::write_arg<cpf::type::str_t>(	ustream, format, std::forward<cpf::type::str_t>(cpf::intern::wconv(reinterpret_cast<char*>(arg))));
}

template<>
void cpf::intern::write_arg<const signed char*>(cpf::type::stream ustream,
											cpf::type::str_t const &format,
											const signed char*&& arg)
{
	cpf::intern::write_arg<cpf::type::str_t>(	ustream, format, std::forward<cpf::type::str_t>(cpf::intern::wconv(reinterpret_cast<const char*>(arg))));
}

CPF_API void cpf::intern::update_ustream(	cpf::type::stream ustream,
											const cpf::type::meta_fmt_t::const_iterator &end_point_comparator,
											cpf::type::meta_fmt_t::const_iterator &meta_iter,
											const cpf::type::str_t printed_string=L"",
											const cpf::type::size search_start_pos=0)
{
	while (meta_iter != end_point_comparator)
    {
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

