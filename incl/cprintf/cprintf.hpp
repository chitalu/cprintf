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

template< std::size_t FLAGS = CPF_STDO, typename F = cpf::type::str, typename... Fs>
auto cprintf(F f, Fs... args) -> cpf::type::ret_t<F>
{
	static_assert(	(((FLAGS bitand CPF_STDO) == CPF_STDO) and	((FLAGS bitand CPF_STDE) != CPF_STDE)) or
					(((FLAGS bitand CPF_STDO) != CPF_STDO) and	((FLAGS bitand CPF_STDE) == CPF_STDE)),
					"CPF-CT-ERR: invalid stream specification");

	static_assert(	(FLAGS xor CPF_FLAG_MASK_) <= CPF_FLAG_MASK_,
					"CPF-CT-ERR: invalid API flags detected");

	//TODO: resolve this
	//typename std::result_of<decltype(cprintf<FLAGS, F, Fs...>(f, args...))>::type ret;
	cpf::type::ret_t<F> ret; ret.f = f;

	CPF_MARK_CRITICAL_SECTION_;
	{
		ret.c = cpf::intern::dispatch(	((FLAGS bitand CPF_STDO) == CPF_STDO) ? stdout : stderr,
										std::forward<F>(f),
										std::forward<Fs>(args)...);
	}
	CPF_UNMARK_CRITICAL_SECTION_;

	return ret;
}

template<std::size_t FLAGS = CPF_STDO, typename T>
inline auto x_API_impl(typename std::enable_if<std::is_floating_point<T>::value, T>::type &&arg0) -> cpf::type::ret_t<cpf::type::str>
{
	return cprintf<FLAGS>(cpf::type::str(L"%f"), static_cast<double>(arg0));
}

template<std::size_t FLAGS = CPF_STDO, typename T>
inline auto x_API_impl(typename std::enable_if<std::is_signed<T>::value, std::int64_t>::type &&arg0) -> cpf::type::ret_t<cpf::type::str>
{
	return cprintf<FLAGS>(	cpf::type::str(L"%lld"), std::forward<std::int64_t>(arg0));
}

template<std::size_t FLAGS = CPF_STDO, typename T>
inline auto x_API_impl(typename std::enable_if<std::is_unsigned<T>::value, std::uint64_t>::type &&arg0) -> cpf::type::ret_t<cpf::type::str>
{
	return cprintf<FLAGS>(cpf::type::str(L"%llu"),	std::forward<std::uint64_t>(arg0));
}

template<std::size_t FLAGS = CPF_STDO, typename T>
inline auto x_API_impl(typename std::enable_if<std::is_pointer<T>::value, T>::type &&arg0) -> cpf::type::ret_t<cpf::type::str>
{
	return cprintf<FLAGS>(cpf::type::str(L"%p"), std::forward<T>(arg0));
}

template<std::size_t FLAGS = CPF_STDO, typename T>
inline auto cprintf_x(T arg0) -> cpf::type::ret_t<typename std::enable_if<std::is_scalar<T>::value, cpf::type::str>::type>
{
	return x_API_impl<FLAGS, T>(std::forward<T>(arg0));
}

template< std::size_t FLAGS = CPF_STDO, typename F, typename... Fs>
inline auto cprintf_t(F f, cpf::type::arg_pack<Fs...> args_tup) -> cpf::type::ret_t<F>
{
	auto predef_args_tup = std::make_tuple(f);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);

	return cpf::intern::apply_tuple(cprintf<FLAGS, F, Fs...>, call_args);
}

template< std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline auto cprintf_s(F(&f)[N], Fs... args) -> cpf::type::ret_t<F>
{
	static_assert(N >= 2, "CPF-CT-ERR: expected string-literal of size >= 1");
	return cprintf<FLAGS>(	f, std::forward<Fs>(args)...);
}

template<std::size_t FLAGS = CPF_STDO, typename F, unsigned N, typename... Fs>
inline auto cprintf_ts(F(&f)[N], cpf::type::arg_pack<Fs...> args_tup) -> cpf::type::ret_t<F>
{
	static_assert(N >= 2, "CPF-CT-ERR: expected string-literal of size >= 1");
	return cprintf_t<FLAGS>(f,	std::forward<cpf::type::arg_pack<Fs...>>(args_tup));
}

#include <cprintf/internal/cpf_dbgh.h>

#endif /* __CPF_H__ */
