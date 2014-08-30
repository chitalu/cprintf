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

#include "cprintf/cprintf.h"
#include "_cpf_config.h"
#include "_cpf_find.h"

/*text attributes before a call was made to cprintf*/
cpf::type::colour _cpf_default_sys_attribs = SYSTXTATTRIB_UNDEF;

CPF_API const cpf::type::str cpf::pre_debug_log_str =
L"debug_str(\n\
$c\n\
>> cpf debug call \n\
@file:	%s\n\
@built:	%s-%s \n\
\
>	@function:	%s\n\
>	@line:		%d\n\
$?\n\
log:\n\
)debug_str";

//cprintf("Characters:\t%c %%\n", 65);
cpf::type::size cpf::get_num_arg_specs(const cpf::type::str & obj)
{
	cpf::type::size n = 0;
	std::int32_t pos = 0;
	while ((pos = cpf::search_for(L"%", obj, pos, '%')) != cpf::type::str::npos)
	{
		if (pos == obj.size() - 1)
		{
			/*this would imply the following: cprintf("foo bar %");*/
			throw cpf::type::except(L"invalid format specifier ('%') position.");
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

cpf::type::str cpf::write_pre_arg_str(	cpf::type::stream ustream,
										cpf::type::str& printed_string_,
										cpf::type::size& ssp_,
										const cpf::type::attribute_group attr)
{
	cpf::configure(ustream, attr);

	ssp_ = cpf::search_for(L"%", printed_string_, ssp_, '%');
	if (ssp_ != 0)
	{
#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fwprintf(ustream, printed_string_.substr(0, ssp_).c_str());
#ifdef __gnu_linux__
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
	for (const auto &bfs : cpf::std_format_specifiers)
	{
		if (printed_string_[ssp_ + 1] == bfs)
		{
			offset = 2;
			break;
		}
	}

	if (offset == 0)
	{
		cpf::type::str specifier;
		bool parsed_complete_f_spec = false;
		for (const auto &xfst : cpf::extended_format_specifier_terminators)
		{
			auto ps = printed_string_.substr(ssp_ + 1);
	
			auto _max = (ps.size());
			for (auto i(0u); i < _max; ++i)
			{
				auto crnt_char = ps[i];
				if (isalnum(crnt_char) || is_in(crnt_char, cpf::intermediate_format_specifers) || crnt_char == xfst)
				{
					specifier.append({ crnt_char });

					if (crnt_char == xfst || is_in(crnt_char, cpf::extended_format_specifier_terminators))
					{
						parsed_complete_f_spec = true;
						break;
					}
				}
				else
				{
					throw cpf::type::except(L"cpf err: invalid format specifier detail");
				}

				//last iteration
				if (i == (_max - 1) && !is_in(crnt_char, cpf::extended_format_specifier_terminators))
				{
					throw cpf::type::except(L"cpf err: invalid format specifier");
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

void cpf::write_post_arg_str(	cpf::type::stream ustream,
								cpf::type::str& printed_string_,
								cpf::type::size& ssp_,
								bool &more_args_on_iter,
								cpf::type::meta::const_iterator &meta_iter,
								const cpf::type::meta::const_iterator &end_point_comparator)
{
	printed_string_ = printed_string_.substr(ssp_);
	ssp_ = 0;

	more_args_on_iter = cpf::get_num_arg_specs(printed_string_) > 0;
	if (!more_args_on_iter)
	{
		if (!printed_string_.empty())
		{
#ifdef __gnu_linux__
/*see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
			fwprintf(ustream, printed_string_.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
			printed_string_.clear();
		}
		std::advance(meta_iter, 1);
	}
}

void cpf::write_non_arg_str(cpf::type::stream ustream,
							cpf::type::str& printed_string_,
							cpf::type::size& ssp_,
							cpf::type::meta::const_iterator &meta_iter)
{
	cpf::configure(ustream, meta_iter->second.first);

	ssp_ = 0;

#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
	fwprintf(ustream, printed_string_.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
	std::advance(meta_iter, 1);

	while (cpf::get_num_arg_specs(meta_iter->second.second) == 0)
	{
		cpf::configure(ustream, meta_iter->second.first);
#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fwprintf(ustream, meta_iter->second.second.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
		std::advance(meta_iter, 1);
	}
}

template<>
void cpf::write_arg<cpf::type::str>(	cpf::type::stream ustream, 
										cpf::type::str const &format, 
										cpf::type::str&& arg)
{
	fwprintf(ustream, format.c_str(), arg.c_str());
}

void cpf::call_(	
	cpf::type::stream ustream,
	const cpf::type::meta::const_iterator &end_point_comparator,
	cpf::type::meta::const_iterator &meta_iter,
	const cpf::type::str printed_string=L"",
	const cpf::type::size search_start_pos=0)
{
	while (meta_iter != end_point_comparator)
    {
		cpf::configure(ustream, meta_iter->second.first);

#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

		fwprintf(ustream, meta_iter->second.second.c_str());

#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
		std::advance(meta_iter, 1);
    }

	/*restore defaults*/
	configure(ustream, cpf::type::string_vector({L"?"}));
}
