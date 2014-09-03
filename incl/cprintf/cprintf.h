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

#ifndef NDEBUG
#include <cstdarg> //only used by debug functions
#endif

#include <tuple>
#include <memory>
#include <algorithm>

#ifndef __gnu_linux__
/*GCC does not yet support multi-byte conversion functionality from this header
as a result narrow-string variants of cprintf's API will do anything until this
is resolved*/
#include <codecvt> //wstring_convert
#endif

#include "cprintf/internal/_cpf_parse.h"
#include "cprintf/internal/_cpf_verify.h"
#include "cprintf/internal/_cpf_config.h"

namespace cpf
{
	template <std::size_t...>
	struct indices
	{	 };

	template <	std::size_t Begin,
				std::size_t End,
				typename Indices = indices<>,
				typename Enable = void>
	struct make_seq_indices
	{
		static_assert(Begin <= End, "Begin must be <= End");
	};

	template <	std::size_t Begin,
				std::size_t End,
				template <std::size_t...> class I,
				std::size_t... Indices>
	struct make_seq_indices<Begin, 
							End, 
							I<Indices...>,
							typename std::enable_if<Begin < End, void>::type>
	{
		using type = typename make_seq_indices<Begin + 1, End, I<Indices..., Begin>>::type;
	};

	template <std::size_t Begin,std::size_t End,typename Indices>
	struct make_seq_indices<Begin, 
							End,
							Indices,
							typename std::enable_if<Begin == End, void>::type>
	{
		using type = Indices;
	};

	/*
		to obtain a list of indices.The type alias is defined as :
	*/
	template <std::size_t Begin, std::size_t End>
	using make_seq_indices_T = typename make_seq_indices<Begin, End>::type;

	/*
		It is useful to consider how to pass a set of function arguments to a 
		function or functor.The code to do this is:

		**constexpr
	*/
	template <typename Op, typename... Args>
		inline auto apply(Op&& op, Args&&... args) -> 
	decltype(std::forward<Op>(op)(std::forward<Args>(args)...))
	{
		return std::forward<Op>(op)(std::forward<Args>(args)...);
	}

	/*
		This function overload applies op to all tuple indices...
	*/
	template <	typename Op,
				typename Tuple,
				template <std::size_t...> class I,
				std::size_t... Indices>
	inline auto _apply_tuple(Op&& op, Tuple&& t, I<Indices...>&&) -> 
	decltype(std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...))
	{
		return std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...);
	}

	/* 
		This function overload forwards op and t along with the
		indices of the tuple generated by make_seq_indices...
	*/
	template <	typename Op,
				typename Tuple,
				typename Indices =	make_seq_indices_T<	0,std::tuple_size<typename std::decay<Tuple>::type>::value>>
	inline auto apply_tuple(Op&& op, Tuple&& t) -> 
	decltype(_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{}))
	{
		return	_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{});
	}

	/*
		@return number of printf argument tokens "%" in a given string
	*/
	CPF_API cpf::type::size get_num_arg_specs(	const cpf::type::str & str);

	/*
		print the substring preceding an argument specifier in a sub-format-string
	*/
	CPF_API cpf::type::str write_pre_arg_str(	cpf::type::stream ustream,
												cpf::type::str& printed_string_,
												cpf::type::size& ssp_,
												const cpf::type::attribute_group attr);

	/*
		print the substring proceding an argument specifier in a sub-format-string 
	*/
	CPF_API void write_post_arg_str(cpf::type::stream ustream,
									cpf::type::str& printed_string_,
									cpf::type::size& ssp_,
									bool &more_args_on_iter,
									cpf::type::meta::const_iterator &meta_iter,
									const cpf::type::meta::const_iterator &end_point_comparator);

	/*
		print non-argument specifying format string i.e where the implmentation
		need not invoke printf with any avariadic arguments.
	*/
	CPF_API void write_non_arg_str(	cpf::type::stream ustream,
									cpf::type::str& printed_string_,
									cpf::type::size& ssp_,
									cpf::type::meta::const_iterator &meta_iter);

	template<typename T>
	void write_arg(	cpf::type::stream ustream, 
					cpf::type::str const &format, 
					T&& arg)
	{
		fwprintf(ustream, format.c_str(), arg);
	}

	template<>
	CPF_API void write_arg<cpf::type::str>(	cpf::type::stream ustream,
											cpf::type::str const &frmt, 
											cpf::type::str&& arg);

	/*template<>not yet supported
	CPF_API void write_arg<cpf::type::nstr>(cpf::type::stream ustream,
											cpf::type::str const &frmt,
											cpf::type::nstr&& arg);*/

	/*
		recursion-terminating function (counterpart to call_ with variadic arguments). 
		This is the function executated when cprintf is called with only a format 
		string and no arguments.
	*/
	CPF_API void call_(	cpf::type::stream ustream,
						const cpf::type::c_meta_iterator &end_point_comparator,
						cpf::type::c_meta_iterator &meta_iter,
						const cpf::type::str printed_string,
						const cpf::type::size search_start_pos);

	/*
		recursive call to process the format string as well as every argument provided.
		note: this function is not executed if no variadic arguments are respecified.
		using cfprintf_t and cprintf_t guarrantees the execution of this function.
	*/
	template<typename T0, typename ...Ts>
	void call_(	cpf::type::stream ustream,
				const cpf::type::c_meta_iterator &end_point_comparator,
				cpf::type::c_meta_iterator &meta_iter,
				const cpf::type::str printed_string,
				const cpf::type::size search_start_pos,
				T0&& arg0,
				Ts&&... args)
	{
		cpf::type::str printed_string_ = printed_string;

		/*	printed string argument-specifier ('%') count	*/
		const auto pstr_argc = cpf::get_num_arg_specs(printed_string_);
	
				/*	more printf args to print using "printed_string_" as 
					format string.
				*/
		bool 	more_args_on_iter = false,
				/*	boolean used to determine whether variadic arg0 has been 
					passed to printf as an argument yet	
				*/
				printed_arg0 = false;

		/*	string parsing start position...	*/
		auto ssp_ = search_start_pos;

		if (pstr_argc >= 1)
		{
			auto arg_format_spec = cpf::write_pre_arg_str(	ustream, 
															printed_string_, 
															ssp_, 
															meta_iter->second.first);
			cpf::write_arg(	ustream, 
							arg_format_spec,
							std::forward<T0>(arg0));
			cpf::write_post_arg_str(ustream, 
									printed_string_, 
									ssp_, 
									more_args_on_iter,
									meta_iter, 
									end_point_comparator);
			printed_arg0 = true;
		}
		else
		{
			cpf::write_non_arg_str(ustream, printed_string_, ssp_, meta_iter);
		}

		bool iter_reached_end = (meta_iter == end_point_comparator);
		auto i_raw_str = iter_reached_end ? L"" : meta_iter->second.second;

		/*
			note: 	only when "arg0" has been passed to fprintf does 
					variadic-argument iteration proceed onto the 
					next one subsequently after arg0. Else recurse back
					into this function with the same arguments.
		*/
		if (printed_arg0)
		{
			cpf::call_(	ustream, end_point_comparator, meta_iter,
						(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
						more_args_on_iter, std::forward<Ts>(args)...);
		}
		else
		{
			cpf::call_(	ustream, end_point_comparator, meta_iter,
						(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
						ssp_, arg0, std::forward<Ts>(args)...);
		}
	}

	CPF_API const cpf::type::nstr pre_debug_log_str;
}

template<typename... Ts>
void cfwprintf(cpf::type::stream ustream, const wchar_t* format, Ts... args)
{
	assert(ustream != nullptr && "cpf err: output stream is undefined (null)");

	auto meta_format = cpf::process_format_string(format);

#ifndef NDEBUG
	
	cpf::type::size nargs = 0u;
	for (const auto &i : meta_format)
	{
		nargs += cpf::get_num_arg_specs(i.second.second);
	}

	if (nargs != sizeof...(args))
	{
		throw cpf::type::except(L"cpf err: invalid argument count");
	}

	/*
		debug-build call signature verification 
	*/
	//_cpf_verify(format, normalize_arg(args)...);
#endif
	
	auto mf_begin = meta_format.cbegin();
	auto mf_endpoint_cmp = meta_format.cend();/*end point comparator...*/

	/*			*/
	try
	{
		cpf::save_stream_state(ustream);

		/*	make actual call to do printing and internal colour configurations	*/
		cpf::call_(	ustream,
					mf_endpoint_cmp,
					mf_begin,
					mf_begin->second.second,
					0u,
					std::forward<Ts>(cpf::normalize_arg(args))...);
	}
	catch (cpf::type::except &e)
	{
		/*TODO: look into signal interupts. they are not covered here*/
		cpf::restore_stream_state(ustream, true);
		throw e;
	}
	cpf::restore_stream_state(ustream, true);
}

/*
Writes the string referenced by "format" to the "stream". If "format"
includes format specifiers (subsequences beginning with %), the
additional cprintf arguments following "format" are formatted and inserted
in the resulting string replacing their respective specifiers.
If format includes attribute specifiers (tokens beginning with $),
the characters proceding these attibute specifiers will be modified
in accordance with specification contained in the format.

After the format parameter, the function expects at least as many
additional arguments as specified by "format".

@stream
Pointer to a FILE object that identifies an output stream.
@format
string that contains the text to be written to the stream.
It can optionally contain embedded format specifiers that
are replaced by the values specified in subsequent additional
arguments and formatted as requested or are used as indicators
of how the output text should appear in a console..

For further info please refer to:
http://www.cplusplus.com/reference/cstdio/fprintf/
*/
template<typename... Ts>
void cfprintf(cpf::type::stream ustream, const char* format, Ts... args)
{
	/*
		GCC does not yet support multi-byte conversion functionality...

		http://stackoverflow.com/questions/24497956/is-codecvt-not-supported-by-clang-or-gcc
		http://stackoverflow.com/questions/15615136/is-codecvt-not-a-std-header?rq=1
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/facets.html#std.localization.facet.codecvt
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011
	*/
#ifdef __gnu_linux__
		printf("LINUX IMPLMENTATION AWAITING \"codecvt\" AVAILABILITY!");
	return; //skip
#else
	/*
		*\http://en.cppreference.com/w/cpp/locale/codecvt_utf8
		*\http://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
	*/
	auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();
	auto multibyte_version = converter.from_bytes(format);
	cfwprintf(ustream, multibyte_version.c_str(), args...);
#endif
}

template<typename... Ts>
void cwprintf(const wchar_t* format, Ts... args)
{
	cfwprintf(stdout, format, std::forward<Ts>(args)...);
}

/*
	Writes the string referenced by "format" to "stdout". If "format"
	includes format specifiers (subsequences beginning with %), the
	additional cprintf arguments following "format" are formatted and inserted
	in the resulting string replacing their respective specifiers.
	If format includes attribute specifiers (tokens beginning with $),
	the characters proceding these attibute specifiers will be modified
	in accordance with specification contained in the format.

	After the format parameter, the function expects at least as many
	additional arguments as specified by "format".

	@format
	string that contains the text to be written to stdout.
	It can optionally contain embedded format and attribute specifiers that are replaced
	by the values specified in subsequent additional arguments and
	formatted as requested or are used as indicators of how the output text
	should appear in a console.
*/
template<typename... Ts>
void cprintf(const char* format, Ts... args)
{
	cfprintf(stdout, format, std::forward<Ts>(args)...);
}

template<typename... Ts>
void cfwprintf_t(	cpf::type::stream ustream, 
					const wchar_t* format,
					cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	cpf::apply_tuple(cfwprintf<Ts...>, call_args);
}


/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those permitted to cfprintf.

	see cfprintf doc-string for more info.
*/
template<typename... Ts>
void cfprintf_t(cpf::type::stream ustream, 
				const char* format,
				cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	cpf::apply_tuple(cfprintf<Ts...>, call_args);
}

template<typename... Ts>
void cwprintf_t(const wchar_t* format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfwprintf_t(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those of cprintf.
	all output is directed to stdout.

	see cprintf doc-string for more info.
*/
template<typename... Ts>
void cprintf_t(cpf::type::nstr format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfprintf_t(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

#ifndef	NDEBUG

/*
	The following are auxillary macros ideal for debugging purposes.
	All output is streamed to standard except.
	Users may use these just as they would cprintf,
	cfprintf, cprintf_t, and cfprintf_t. Permissions
	and limitations imposed reflect those of the aforementioned.
	Macro expansion will only occur in client debug builds and non else.
	And as such, building in release mode results in the macros
	expanding to [nothing], rendering a client call impotent.
*/

#ifdef _WIN32
#define CPF_SEP_COND (character == '\\' || character == '/');
#else
#define CPF_SEP_COND ( character == '/');
#endif

/*Note the the anonymous struct for os specific dir path wrangling*/
#define CPF_DBG_LOG_STR\
	cpf::type::nstr const& pathname = __FILE__;\
	typedef struct {\
	bool operator()(char character) const{\
		return CPF_SEP_COND;\
	}\
	}fpath_sep_func;\
	auto fname =  cpf::type::nstr(\
	std::find_if(pathname.rbegin(), pathname.rend(),fpath_sep_func()).base(),\
	pathname.end());\
	cfprintf(stderr, cpf::pre_debug_log_str.c_str(), \
	fname.c_str(), __TIME__, __DATE__, __FUNCTION__, __LINE__);

#define cfprintf_dbg(ustream, format, ...) \
	do{\
	CPF_DBG_LOG_STR \
	cfprintf(ustream, format, ##__VA_ARGS__);\
	}while (0);

#define cprintf_dbg(format, ...) \
	cfprintf_dbg(stderr, format, ##__VA_ARGS__)

#define cfprintf_t_dbg(ustream, format, tup) \
	do{\
	CPF_DBG_LOG_STR \
	cfprintf_t(ustream, format, tup); \
	} while (0);\

#define cprintf_t_dbg(format, tup) \
	cfprintf_t_dbg(stderr, format, tup);

#define cfwprintf_dbg(ustream, format, ...) 
#define cwprintf_dbg(format, ...) 
#define cfwprintf_t_dbg(ustream, format, tup) 
#define cwprintf_t_dbg(format, tup) 


#else

/*do nothing*/
#define cfwprintf_dbg(ustream, format, ...) 
#define cwprintf_dbg(format, ...) 
#define cfwprintf_t_dbg(ustream, format, tup) 
#define cwprintf_t_dbg(format, tup) 

#define cfprintf_dbg(ustream, format, ...) 
#define cprintf_dbg(format, ...) 
#define cfprintf_t_dbg(ustream, format, tup) 
#define cprintf_t_dbg(format, tup) 

#endif /*#ifndef NDEBUG*/

#endif /* __CPF_H__ */