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

#include <tuple>
#include <algorithm>
#include <cstdarg>
#include <cassert>
#include "_cpf_parse.h"
#include "_cpf_type_norm.h"
#include "_cpf_verify.h"

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
	using type =typename make_seq_indices<Begin + 1, End, I<Indices..., Begin>>::type;
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
extern "C" std::size_t _cpf_get_num_arg_specifiers(	const _cpf_type::str & obj, 
													const _cpf_type::str & str);

/*
	print the substring preceding an argument specifier in a sub-format-string
*/
extern _cpf_type::str _cpf_print_pre_arg_str(_cpf_type::stream strm,
														_cpf_type::str& printed_string_,
														std::size_t& ssp_,
														const _cpf_type::attribs attr);

/*
	print the substring proceding an argument specifier in a sub-format-string 
*/
extern void _cpf_print_post_arg_str(_cpf_type::stream strm,
									_cpf_type::str& printed_string_,
									std::size_t& ssp_,
									bool &more_args_on_iter,
									_cpf_type::meta_format_type::const_iterator &msd_iter);

/*
	print non-argument specifying format string i.e where the implmentation
	need not invoke printf with any avariadic arguments.
*/
extern void _cpf_print_non_arg_str(	_cpf_type::stream strm,
									_cpf_type::str& printed_string_,
									std::size_t& ssp_,
									_cpf_type::meta_format_type::const_iterator &msd_iter);

/*
	recursion terminating function (counterpart to _cpf_call with variadic arguments). 
	This is the function executated when c_printf is called with only a format 
	string and no arguments.
*/
extern void _cpf_call_(	
	_cpf_type::stream strm,
	const _cpf_type::meta_format_type::const_iterator &end_point_comparator,
	_cpf_type::meta_format_type::const_iterator &msd_iter,
	const _cpf_type::str printed_string,
	const std::size_t search_start_pos);

/*
	recursive call to process the format string as well as every argument provided.
	note: this function is not executed if no variadic arguments are respecified.
	using c_fprintf_t and c_printf_t guarrantees the execution of this function.
*/
template<typename T0, typename ...Ts>
void _cpf_call_(	
	_cpf_type::stream strm,
	const _cpf_type::meta_format_type::const_iterator &end_point_comparator,
	_cpf_type::meta_format_type::const_iterator &msd_iter,
    const _cpf_type::str printed_string,
	const std::size_t search_start_pos,
	T0&& arg0,
    Ts&&... args)
{
	_cpf_type::str printed_string_ = printed_string;

	/*
		printed string argument-specifier ('%') count
	*/
	const auto pstr_argc = _cpf_get_num_arg_specifiers(printed_string_, "%");
	
			/*
				more printf args to print using "printed_string_" as 
				format string.
			*/
	bool 	more_args_on_iter = false,
			/*	boolean used to determine whether variadic arg0 has been 
				passed to printf as an argument yet	
			*/
			printed_arg0 = false;

	

	/*
		string parsing start position...
	*/
    auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		auto fstr = _cpf_print_pre_arg_str(strm, printed_string_, ssp_, msd_iter->second.first);
		fprintf(strm, fstr.c_str(), arg0);
		_cpf_print_post_arg_str(strm, printed_string_, ssp_, more_args_on_iter, msd_iter);
		printed_arg0 = true;
	}
	else
	{
		_cpf_print_non_arg_str(strm, printed_string_, ssp_, msd_iter);
	}

	bool iter_reached_end = (msd_iter == end_point_comparator);
	auto i_raw_str = iter_reached_end ? "" : msd_iter->second.second;

	/*
		note: 	only when "arg0" has been passed to fprintf does 
				variadic-argument iteration proceed onto the 
				next one subsequently after arg0. Else recurse back
				into this function with the same arguments.
	*/
	if (printed_arg0)
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
            more_args_on_iter, std::forward<Ts>(args)...);
    }
	else
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
			ssp_, arg0, std::forward<Ts>(args)...);
    }
}

/*
	Writes the C string pointed by format to the stream. If format 
	includes format specifiers (subsequences beginning with %), the 
	additional arguments following format are formatted and inserted
	in the resulting string replacing their respective specifiers.
	If format includes attribute specifiers (tokens beginning with /),
	the characters proceding these attibute specifiers will be modified 
	in accordance with specification contained in the format.

	After the format parameter, the function expects at least as many 
	additional arguments as specified by format.

	@stream
    	Pointer to a FILE object that identifies an output stream.
	@format
	    C string that contains the text to be written to the stream.
	    It can optionally contain embedded format specifiers that 
	    are replaced by the values specified in subsequent additional 
	    arguments and formatted as requested or are used as indicators 
		of how the output text should appear in a console..

	For further info please refer to:
	http://www.cplusplus.com/reference/cstdio/fprintf/
*/
template<typename... Ts>
void c_fprintf(_cpf_type::stream strm, _cpf_type::c_str format, Ts... args)
{
#if defined(_DEBUG)
	assert(strm != nullptr && "output stream undefined");
	assert(format != nullptr && "format string undefined");

	/*
		debug-build call signature verification 
	*/
	//_cpf_verify(format, normalize_arg(args)...);
#endif
	auto meta_str_data = _cpf_process_format_string(format);
	auto tsd_iter_begin = meta_str_data.cbegin();
	auto tsd_iter_end_point_comparator = meta_str_data.cend();

	_cpf_call_(strm,
		tsd_iter_end_point_comparator,
		tsd_iter_begin,
		tsd_iter_begin->second.second,
		0,
		std::forward<Ts>(normalize_arg(args))...);
}

/*
	Writes the C string pointed by format to the standard output (stdout). 
	If format includes format specifiers (subsequences beginning with %), 
	the additional arguments following format are formatted and inserted 
	in the resulting string replacing their respective specifiers.
	If format includes attribute specifiers (tokens beginning with /),
	the characters proceding these attibute specifiers will be modified in accordance
	with specification contained in the format. 

	@format
    C string that contains the text to be written to stdout.
    It can optionally contain embedded format and attribute specifiers that are replaced 
    by the values specified in subsequent additional arguments and 
    formatted as requested or are used as indicators of how the output text 
	should appear in a console.
*/
template<typename... Ts>
void c_printf(_cpf_type::c_str format, Ts... args)
{
	c_fprintf(stdout, format, std::forward<Ts>(args)...);
}

/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those permitted to c_fprintf. 

	see c_fprintf documentation for more info.
*/
template<typename... Ts>
void c_fprintf_t(	_cpf_type::stream strm, 
					_cpf_type::c_str format, std::tuple<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(strm, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	
	apply_tuple(c_fprintf<Ts...>, call_args);
}

/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those permitted to c_printf. 
	all output is directed to stdout.

	see c_printf documentation for more info.
*/
template<typename... Ts>
void c_printf_t(_cpf_type::c_str format, std::tuple<Ts...> args_tup)
{
	c_fprintf_t(stdout, format, std::forward<std::tuple<Ts...>>(args_tup));
}

#ifdef _DEBUG

const auto _cpf_debug_pre_str =
R"debug_str(
/$%s;%d|c*]
>> dbg print 
@build:		%s-%s 
@file:		%s
@function:	%s
@line-number:	%d

log:
)debug_str";

/*
	os specific dir path wrangling
*/
struct _cpf_dbg_fpath_separator
{
	bool operator()(char character) const
	{ 
#ifdef _WIN32
		return character == '\\' || character == '/'; 
#else
		return character == '/';
#endif
	}
};

/*
	The following are auxillary macros ideal for debugging purposes.
	All output is streamed to standard error.
	Users may use these just as they would c_printf,
	c_fprintf, c_printf_t, and c_fprintf_t. Permissions
	and limitations imposed reflect those of the aforementioned.
	Macro expansion will only occur in client debug builds and non else.
	And as such, building in release mode results in the macros
	expanding to [nothing], rendering a client call impotent.
*/

#define __print_stat_str\
	_cpf_type::str const& pathname = __FILE__;\
	auto fname =  _cpf_type::str(\
	std::find_if(pathname.rbegin(), pathname.rend(),\
	_cpf_dbg_fpath_separator()).base(),\
	pathname.end());\
	c_fprintf(stderr, _cpf_debug_pre_str, __TIME__, __DATE__, fname.c_str(), __LINE__, __FUNCTION__); \

#define c_fprintf_dbg(strm, format, ...) \
	do{\
	__print_stat_str \
	c_fprintf(strm, format, ##__VA_ARGS__);\
	}while (0);

#define c_printf_dbg(format, ...) \
	c_fprintf_dbg(stderr, format, ##__VA_ARGS__)

#define c_fprintf_t_dbg(strm, format, tup) \
	do{\
	__print_stat_str \
	c_fprintf_t(strm, format, tup); \
	} while (0);\

#define c_printf_t_dbg(format, tup) \
	c_fprintf_t_dbg(stderr, format, tup);

#else

/*do nothing*/
#define c_fprintf_dbg(strm, format, ...) 
#define c_printf_dbg(format, ...) 
#define c_fprintf_t_dbg(strm, format, tup) 
#define c_printf_t_dbg(format, tup) 

#endif

#endif /* __CPF_H__ */