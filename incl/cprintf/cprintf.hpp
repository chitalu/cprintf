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

// root API

template<	std::size_t FLAGS = CPF_STDO, 
			typename F = cpf::type::str, 
			typename... Fs>
std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
// TODO: Add documentation
cprintf(F f, Fs... args)
{
	using namespace cpf::type;	using namespace cpf::intern;
	// TODO:
	// typedef typename std::result_of<decltype(cprintf<FLAGS, F, Fs...>(f, args...))>::type return_type;
	typedef std::unique_ptr<status_t<verify_<FLAGS, F, Fs...>>> ret_t;
	
	ret_t r(new typename ret_t::element_type);

	std::get<0>(r->cpf_arg) = f;

	CPF_MARK_CRITICAL_SECTION_;
	{
		r->c = dispatch<FLAGS>(std::forward<F>(f), std::forward<Fs>(args)...);
	}
	CPF_UNMARK_CRITICAL_SECTION_;

	return std::move(r);
}

// tuple API

template< std::size_t FLAGS = CPF_STDO, typename F, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
// TODO: Add documentation
cprintf_t(F f, cpf::type::arg_pack<Fs...> args_tup)
{
	auto predef_args_tup = std::make_tuple(f);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);

	return std::move(cpf::intern::apply_tuple(cprintf<FLAGS, F, Fs...>, call_args));
}

// string-literal API

template< std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
// TODO: Add documentation
cprintf_s(F(&f)[N], Fs... args)
{
	static_assert(N >= 2, "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");
	return cprintf<FLAGS>(	f, std::forward<Fs>(args)...);
}

template<std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, F, Fs...>>>
// TODO: Add documentation
cprintf_ts(F(&f)[N], cpf::type::arg_pack<Fs...> args_tup)
{
	static_assert(N >= 2, "CPRINTF COMPILATION ERROR: expected string-literal of size >= 1");
	return cprintf_t<FLAGS>(f,	std::forward<cpf::type::arg_pack<Fs...>>(args_tup));
}

// X api

namespace cpf
{
	namespace intern
	{
		template<std::size_t FLAGS = CPF_STDO, typename T>
		inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str, T>>>
			x_impl(typename std::enable_if<std::is_floating_point<T>::value, T>::type &&arg0)
		{
			return std::move(cprintf<FLAGS>(cpf::type::str(L"%f"), arg0));
		}

		template<std::size_t FLAGS = CPF_STDO, typename T>
		inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str, T>>>
			x_impl(typename std::enable_if<std::is_signed<T>::value, T>::type &&arg0)
		{
				return std::move(cprintf<FLAGS>(cpf::type::str(L"%lld"), arg0));
		}

		template<std::size_t FLAGS = CPF_STDO, typename T>
		inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str, T>>>
			x_impl(typename std::enable_if<std::is_unsigned<T>::value, T>::type &&arg0)
		{
			return std::move(cprintf<FLAGS>(cpf::type::str(L"%llu"), arg0));
		}

		template<std::size_t FLAGS = CPF_STDO, typename T>
		inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str, T>>>
			x_impl(typename std::enable_if<std::is_pointer<T>::value, T>::type &&arg0)
		{
			return std::move(cprintf<FLAGS>(cpf::type::str(L"%p"), arg0));
		}
	}
}

template<typename T>
struct resolve_integral_t
{
	typedef typename std::conditional<
			std::is_signed<T>::value,
			std::int64_t,
			std::uint64_t
		>::type type;
};

template<typename T>
struct xpromote_t
{
	cpf::type::verify_args_<T> decl_stub_;

	typedef typename std::conditional<
		std::is_integral<T>::value,
		typename resolve_integral_t<T>::type,
		typename std::conditional<
			std::is_floating_point<T>::value,
			double,
			typename std::conditional<
				std::is_pointer<T>::value,
				T,
				cpf::type::stub_t
			>::type
		>::type
	>::type type;
};

template<std::size_t FLAGS = CPF_STDO, typename T>
inline std::unique_ptr<cpf::type::status_t<cpf::type::verify_<FLAGS, cpf::type::str, typename xpromote_t<T>::type>>>
// TODO: Add documentation
cprintx(T arg0)
{
	typedef typename xpromote_t<T>::type scalar_t;
	return std::move(cpf::intern::x_impl<FLAGS, scalar_t>(std::forward<scalar_t>(static_cast<scalar_t>(arg0))));
}

#include <cprintf/internal/cpf_dbgh.h>

#endif /* __CPF_H__ */
