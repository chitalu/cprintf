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
_cpf_type::colour _cpf_default_sys_attribs = SYSTXTATTRIB_UNDEF;

//cprintf("Characters:\t%c %%\n", 65);
std::size_t _cpf_get_num_arg_specifiers(const _cpf_type::str & obj)
{
	std::size_t n = 0;
	std::int32_t pos = 0;
	while ((pos = _cpf_find("%", obj, pos, '%')) != _cpf_type::str::npos)
	{
		if (pos == obj.size() - 1)
		{
			/*this would imply the following: cprintf("foo bar %");*/
			throw _cpf_type::error("invalid format specifier ('%') position.");
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

_cpf_type::str _cpf_print_pre_arg_str(	_cpf_type::stream strm,
													_cpf_type::str& printed_string_,
													std::size_t& ssp_,
													const _cpf_type::attribs attr)
{
	_cpf_config_terminal(strm, attr);

	ssp_ = _cpf_find("%", printed_string_, ssp_, '%');
	if (ssp_ != 0)
	{
#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fprintf(strm, printed_string_.substr(0, ssp_).c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
	}

	auto is_in = [=](const char& _target, const std::initializer_list<char>& _list)->bool
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
	std::size_t offset = 0;
	for (const auto &bfs : std_format_specifiers)
	{
		if (printed_string_[ssp_ + 1] == bfs)
		{
			offset = 2;
			break;
		}
	}

	if (offset == 0)
	{
		std::string specifier;
		bool parsed_complete_f_spec = false;
		for (const auto &xfst : extended_format_specifier_terminators)
		{
			auto ps = printed_string_.substr(ssp_ + 1);
	
			auto _max = (ps.size());
			for (auto i(0u); i < _max; ++i)
			{
				auto crnt_char = ps[i];
				if (isalnum(crnt_char) || is_in(crnt_char, intermediate_format_specifers) || crnt_char == xfst)
				{
					specifier.append({ crnt_char });

					if (crnt_char == xfst || is_in(crnt_char, extended_format_specifier_terminators))
					{
						parsed_complete_f_spec = true;
						break;
					}
				}
				else
				{
					throw _cpf_type::error("cpf err: invalid format specifier detail");
				}

				//last iteration
				if (i == (_max - 1) && !is_in(crnt_char, extended_format_specifier_terminators))
				{
					throw _cpf_type::error("cpf err: invalid format specifier");
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

void _cpf_print_post_arg_str(	_cpf_type::stream strm,
								_cpf_type::str& printed_string_,
								std::size_t& ssp_,
								bool &more_args_on_iter,
								_cpf_type::meta_format_type::const_iterator &msd_iter,
								const _cpf_type::meta_format_type::const_iterator &end_point_comparator)
{
	printed_string_ = printed_string_.substr(ssp_);
	ssp_ = 0;

	more_args_on_iter = _cpf_get_num_arg_specifiers(printed_string_) > 0;
	if (!more_args_on_iter)
	{
		if (!printed_string_.empty())
		{
#ifdef __gnu_linux__
/*see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
			fprintf(strm, printed_string_.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
			printed_string_.clear();
		}
		std::advance(msd_iter, 1);
	}
}

void _cpf_print_non_arg_str(_cpf_type::stream strm,
							_cpf_type::str& printed_string_,
							std::size_t& ssp_,
							_cpf_type::meta_format_type::const_iterator &msd_iter)
{
	_cpf_config_terminal(strm, msd_iter->second.first);

	ssp_ = 0;

#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
	fprintf(strm, printed_string_.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
	std::advance(msd_iter, 1);

	while (_cpf_get_num_arg_specifiers(msd_iter->second.second) == 0)
	{
		_cpf_config_terminal(strm, msd_iter->second.first);
#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
		fprintf(strm, msd_iter->second.second.c_str());
#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
		std::advance(msd_iter, 1);
	}
}

void _cpf_call_(	
	_cpf_type::stream strm,
	const _cpf_type::meta_format_type::const_iterator &end_point_comparator,
	_cpf_type::meta_format_type::const_iterator &msd_iter,
	const _cpf_type::str printed_string="",
	const std::size_t search_start_pos=0)
{
	while (msd_iter != end_point_comparator)
    {
		_cpf_config_terminal(strm, msd_iter->second.first);

#ifdef __gnu_linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

		fprintf(strm, msd_iter->second.second.c_str());

#ifdef __gnu_linux__
#pragma GCC diagnostic pop
#endif
		std::advance(msd_iter, 1);
    }

	/*restore defaults*/
	
	_cpf_config_terminal(strm, _cpf_type::str_vec({"?"}));
}
