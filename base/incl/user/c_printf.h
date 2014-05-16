#ifndef C_PRINTF_H
#define C_PRINTF_H

#include <tuple>
#include <cstdio>
#include "_cpf_parse.h"
#include "_cpf_type_norm.h"

extern "C" void _cpf_store_sys_default_attribs(_cpf_types::stream strm);

/*

*/
extern "C" std::size_t _cpf_get_num_arg_specifiers(	const _cpf_types::_string_type_ & obj, 
													const _cpf_types::_string_type_ & str);

/*

*/
extern _cpf_types::_string_type_ _cpf_print_pre_arg_str(_cpf_types::stream strm,
														_cpf_types::_string_type_& printed_string_,
														std::size_t& ssp_,
														const _cpf_types::_string_type_ c_repr);

/*

*/
extern void _cpf_print_post_arg_str(_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									bool &more_args_on_iter,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*

*/
extern void _cpf_print_non_arg_str(	_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*

*/
extern void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
	const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos);

/*
	recursive call to process the format string as well as any arguments provided
	note: this function is not executed if no vairadic arguments as respecified
*/
template<typename T0, typename ...Ts>
void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
    const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos,
	T0&& arg0,
    Ts&&... args)
{
	_cpf_types::_string_type_ printed_string_ = printed_string;

	/*printed string argument ('%') count*/
	const auto pstr_argc = _cpf_get_num_arg_specifiers(printed_string_, "%");
	
	/*more printf args to print using "printed_string_" as format	*/
	bool more_args_on_iter = false;

	/*boolean used to determine whether variadic arg0 has been passed to printf as an argument yet	*/
	bool printed_arg0 = false;

	/*string parsing start position...*/
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
	c_printf is a C++(11/0x) language function for extended formatted-printing. 
	It achieves this by providing a thin abstraction layer atop that of fprintf. 
	The function works,	in much a similar manner to that of its patternal 
	counter-part(s) i.e printf, fprintf etc. 
	Aside from guarranteed type-safety (unlike that of it predecesors) c_printf also
	introduces the feature of colour token specification (and other extra formatting tokens). 
	With this, users are able to specify, as part of the format string, the colour of
	all or some of the format string text within console output. Alongside this is also the introduction 
	tagging tokens which enable users the ability to map specific strings with 
	certain token value.

	@the function will throw an invalid_argument exception on condition that an invalid token 
	is encountered on parsing. In the case that a user opts to use features which are not available
	for an implementation, the format string may remain unchanged with a possibility of
	an exception being thrown on parsing.
	 
*/
template<typename... Ts>
void c_printf(	_cpf_types::stream strm, std::string format, Ts... args)
{
	if (strm == nullptr)
	{ 
		throw std::invalid_argument("output stream undefined");
	}
	else if(format.empty())
	{
		throw std::invalid_argument("format undefined");
	}

#if defined(_DEBUG)
	//this will be used in the first parse stage not here!!
	//_cpf_verify(format, normalize_arg(args)...);
#endif
	auto meta_str_data = _cpf_process_format_string(format);
	auto tsd_iter_begin = meta_str_data.cbegin();
	auto tsd_iter_end_point_comparator = meta_str_data.cend();

	_cpf_store_sys_default_attribs(strm);
	_cpf_call_(	strm,
				tsd_iter_end_point_comparator,
				tsd_iter_begin,
				tsd_iter_begin->second.second,
				0,
				std::forward<Ts>(normalize_arg(args))...);
}

#ifdef _DEBUG

#include <stdarg.h>

const auto _cpf_debug_pre_str =
R"debug_str(
>> dbg print 
@build:		%s-%s 
@file:		%s
@line-number:	%d
@function:	%s

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
	Auxillary 'macro-functio'n ideal for debugging purposes
	note: all output is streamed to standard error.
	users may use this function just as they would "c_printf"
	features permitted and limitations imposed reflect those 
	of "c_printf".
	This function will only work for debug builds and non else.
	By design, building in release mode results in the macro-function
	expanding to nothing, rendering your call impotent.

	TODO: IMPLEMENT YOUR OWN PATH STRIPPER INSTEAD OF DEPENDING ON
	std::find_if

#define c_printf_dbg(format, ...) \
	do{\
	std::string const& pathname = __FILE__;\
	auto fname =  std::string(\
	std::find_if(pathname.rbegin(), pathname.rend(),\
	_cpf_dbg_fpath_separator()).base(),\
	pathname.end());\
	fprintf(stderr, _cpf_debug_pre_str, __DATE__, __TIME__, fname.c_str(), __LINE__, __FUNCTION__); \
	}while (0);\
c_printf(stderr, format, ##__VA_ARGS__);
*/

#define c_printf_dbg(format, ...) \
	do{\
	fprintf(stderr, _cpf_debug_pre_str, __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__); \
	c_printf(stderr, format, ##__VA_ARGS__);\
	}while (0);

#else

#define debug_c_printf(format, ...) /*do nothing*/

#endif

#endif /* C_PRINTF_H */