#pragma once 

#ifdef _WIN32
#include <Windows.h>
#endif

#include <array>
#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <cprintf/internal/cpf_base.h>

namespace _cprintf_
{

#ifdef CPF_WINDOWS_BUILD
	typedef WORD system_color_repr_t;
#else
	typedef unicode_string_t system_color_repr_t;
#endif

	typedef std::wstring unicode_string_t;

	typedef std::string ascii_string_t;

	typedef const wchar_t* unicode_character_string_ptr_t;

	typedef std::pair<unicode_string_t, unicode_string_t> unicode_string_pair_t;

	typedef std::vector<unicode_string_t> unicode_string_vector_t;

	typedef unicode_string_vector_t          attribute_group_t;

	typedef std::map<std::int32_t, std::pair<unicode_string_vector_t, unicode_string_t> > format_string_layout_t;

	typedef format_string_layout_t::iterator       format_string_layout_iterator_t;

	typedef format_string_layout_t::const_iterator format_string_layout_const_iterator_t;

	typedef std::map<const unicode_string_t, system_color_repr_t> token_value_storage_t;

	typedef decltype(stdout) file_stream_t;

	// placeholder type
	struct stub_t
	{
	};

	template <bool B>
	using infer_boolean_type_ =
		std::conditional<B, std::true_type, std::false_type>;

	template <typename T>
	struct is_ascii_char_ptr_
		: infer_boolean_type_<std::is_same<char*, T>::value ||
		std::is_same<unsigned char*, T>::value ||
		std::is_same<signed char*, T>::value ||
		std::is_same<const char*, T>::value ||
		std::is_same<const signed char*, T>::value ||
		std::is_same<const unsigned char*, T>::value>::type
	{
	};

	template <typename T>
	struct is_unicode_char_ptr_t
		: infer_boolean_type_<std::is_same<wchar_t*, T>::value ||
		std::is_same<const wchar_t*, T>::value>::type
	{
	};

	template <typename T>
	struct is_ascii_string_type
		: infer_boolean_type_<std::is_same<T, std::string>::value ||
		is_ascii_char_ptr_<T>::value>::type
	{
	};

	template <typename T>
	struct is_unicode_string_type
		: infer_boolean_type_<std::is_same<T, std::wstring>::value ||
		is_unicode_char_ptr_t<T>::value>::type
	{
	};

	template <typename T = stub_t>
	struct is_valid_string_type_
		: infer_boolean_type_<is_unicode_string_type<T>::value ||
		is_ascii_string_type<T>::value>::type
	{
	};

	template <typename T = stub_t>
	struct is_STL_string_type_
		: infer_boolean_type_<std::is_same<T, ascii_string_t>::value ||
		std::is_same<T, unicode_string_t>::value>::type
	{
	};

	template <typename T = int, typename... Ts>
	struct verify_args_
	{
		typedef T current_type;

		typedef typename infer_boolean_type_<
			is_valid_string_type_<current_type>::value ||
			(std::is_arithmetic<current_type>::value ||
				std::is_pointer<current_type>::value)>::type current;

		typedef verify_args_<Ts...> Next;

		static_assert(current::value && Next::current::value, "invalid argument type");
	};
}