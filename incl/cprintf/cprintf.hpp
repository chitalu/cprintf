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

#ifndef __CPF_H__
#define __CPF_H__

#include <cprintf/internal/cpf_hlpr.h>

template<std::size_t FLAGS = CPF_NON, typename... Ts>
void cfwprintf(cpf::type::stream ustream, const cpf::type::str &format, Ts... args)
{
#if CPF_DBG_CONFIG
	if (ustream == nullptr) 
		throw cpf::type::except(L"CPF-RT-ERR: output stream is undefined (null)");

	/*
		format specifier-to-argument correspondence check
		i.e "%d" must correspond to an integral, "%p" to a pointer etc.
	*/
	cpf::intern::arg_check(format.c_str(), std::forward<Ts>(args)...);
#endif

	if (FLAGS & CPF_ATOMIC)
	{
		//lock file object
#ifdef CPF_WINDOWS_BUILD

#elif defined(CPF_LINUX_BUILD)

#endif
	}

	cpf::intern::dispatch(ustream, format, std::forward<Ts>(args)...);

	if (FLAGS & CPF_ATOMIC)
	{
		//unlock file object
#ifdef CPF_WINDOWS_BUILD

#elif defined(CPF_LINUX_BUILD)

#endif
	}
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfprintf(cpf::type::stream ustream, const cpf::type::nstr &format, Ts... args)
{
	/*
		GCC or clang do not yet support multi-byte conversion functionality...

		http://stackoverflow.com/questions/24497956/is-codecvt-not-supported-by-clang-or-gcc
		http://stackoverflow.com/questions/15615136/is-codecvt-not-a-std-header?rq=1
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/facets.html#std.localization.facet.codecvt
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011
	*/
#if defined(CPF_LINUX_BUILD)
	printf("the header \"codecvt\" is missing!\nuse wide char API variants (cwpr...)\n");
	return; //skip
#else
	auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();
	auto multibyte_version = converter.from_bytes(format);

	cfwprintf<FLAGS>(ustream, multibyte_version.c_str(), std::forward<Ts>(args)...);
#endif
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cwprintf(const cpf::type::str &format, Ts... args)
{
	cfwprintf<FLAGS>(stdout, format, std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cprintf(const cpf::type::nstr &format, Ts... args)
{
	cfprintf<FLAGS>(stdout, format, std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfwprintf_t(cpf::type::stream ustream,
						const cpf::type::str &format,
						cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);

	cpf::intern::apply_tuple(cfwprintf<FLAGS, Ts...>, call_args);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfprintf_t(	cpf::type::stream ustream,
						const cpf::type::nstr &format,
						cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);

	cpf::intern::apply_tuple(cfprintf<FLAGS, Ts...>, call_args);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cwprintf_t(const cpf::type::str &format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfwprintf_t<FLAGS>(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cprintf_t(const cpf::type::nstr &format, cpf::type::arg_pack<Ts...> args_tup)
{		
	cfprintf_t<FLAGS>(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfwprintf_s(cpf::type::stream ustream, cpf::type::strl format, Ts... args)
{
	cfwprintf<FLAGS>(	ustream, 
						static_cast<const cpf::type::strl::EType*>(format),
						std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfprintf_s(cpf::type::stream ustream, cpf::type::nstrl format, Ts... args)
{
	cfprintf<FLAGS>(	ustream,
						static_cast<const cpf::type::nstrl::EType*>(format),
						std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cwprintf_s(cpf::type::strl format, Ts... args)
{
	cwprintf<FLAGS>(static_cast<const cpf::type::strl::EType*>(format),
					std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cprintf_s( cpf::type::nstrl format, Ts... args)
{
	cprintf<FLAGS>(	static_cast<const cpf::type::nstrl::EType*>(format), 
					std::forward<Ts>(args)...);
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfwprintf_ts(cpf::type::stream ustream, cpf::type::strl format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfwprintf<FLAGS>(	ustream,
						static_cast<const cpf::type::strl::EType*>(format),
						std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cfprintf_ts(cpf::type::stream ustream, cpf::type::nstrl format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfprintf<FLAGS>(ustream,
					static_cast<const cpf::type::nstrl::EType*>(format),
					std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cwprintf_ts(cpf::type::strl format, cpf::type::arg_pack<Ts...> args_tup)
{
	cwprintf<FLAGS>(static_cast<const cpf::type::strl::EType*>(format),
					std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

template<std::size_t FLAGS = CPF_NON, typename... Ts>
inline void cprintf_ts(cpf::type::nstrl format, cpf::type::arg_pack<Ts...> args_tup)
{
	cprintf<FLAGS>(	static_cast<const cpf::type::nstrl::EType*>(format),
					std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

#include <cprintf/internal/cpf_dbgh.h>

#endif /* __CPF_H__ */
