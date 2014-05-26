#include "cpf_test_common.h"
#include "_cpf_sys_colour_config.h"

#include <sstream>

TEST(Text_Colour_Formatting_Tokens, basic_syntax)
{
	try
	{

		c_printf("%d this /y*]is %s edd", 28, "floyd");

		auto t_str = "left hand side /r*] right hand side";
		_CPF_OSTR_PRINT(printf("%s", t_str));
		ASSERT_NO_THROW(c_printf(t_str));

		auto t_str1 = "left hand side /#y*] right /!]hand side";
		_CPF_OSTR_PRINT(printf("%s", t_str1));
		ASSERT_NO_THROW(c_printf(t_str1));

		auto t_str2 = "left hand side /r*] right /!]hand side";
		_CPF_OSTR_PRINT(printf("%s", t_str2));
		ASSERT_NO_THROW(c_printf(t_str2));

		auto t_str3 = "left hand side /32&] right /!]hand side";
		_CPF_OSTR_PRINT(printf("%s", t_str3));
		ASSERT_NO_THROW(c_printf(t_str3));
	}
	catch (_cpf_type::error& e)
	{
		printf("%s", e.what());
		e;
	}
}

TEST(Text_Colour_Formatting_Tokens, basic_mapping_syntax)
{
	try
	{
		c_printf_dbg("test debug");
		auto t_str = "%d is /$first `;argument;second;foo|r*;#c*;b*] the first ;argument. %s is the second. now changing to foo";
		_CPF_OSTR_PRINT(printf("%s", t_str, 28, "floyd"));
		ASSERT_NO_THROW(c_printf(t_str, 28, "floyd"));

		auto t_str1 = "%d is `/$first;second;foo|g*] the first argument. %s is the second. now changing to foo";
		_CPF_OSTR_PRINT(printf("%s", t_str1, 28, "floyd"));
		ASSERT_NO_THROW(c_printf(t_str1, 28, "floyd"));

		auto t_str2 = "escape chars `/!] hello";
		_CPF_OSTR_PRINT(printf("%s", t_str2));
		ASSERT_NO_THROW(c_printf(t_str2));
	}
	catch (_cpf_type::error& e)
	{
		printf("%s", e.what());
		e;
	}
}

TEST(Text_Colour_Formatting_Tokens, stardard_fg_bg_colour_test)
{
	int count = 0;
	for(auto i : _cpf_std_token_vals )
	{
		ASSERT_NO_THROW(c_printf(
	    std::string("/").append(i.first).append("]").append("%s").append("/!]\t").c_str(), 
	    i.first.c_str()));
	    if((++count % 4) == 0)
	    {
	        printf("\n");
	    }
	}
}

#ifdef __gnu_linux__

TEST(Text_Colour_Formatting_Tokens, basic_full_colour_spectrum_foreground)
{
	auto t_str = "test full spectrum /&32f]colour token\n";
	_CPF_OSTR_PRINT(printf("%s\n", t_str));
	ASSERT_NO_THROW(c_printf( t_str));
}

TEST(Text_Colour_Formatting_Tokens, basic_full_colour_spectrum_background)
{
	auto t_str = "test full spectrum /&bld;32b]colour token\n";
	_CPF_OSTR_PRINT(printf("%s\n", t_str));
	ASSERT_NO_THROW(c_printf( t_str));
}

TEST(Text_Colour_Formatting_Tokens, basic_full_colour_spectrum_foreground_with_attribute)
{
	auto t_str = "test full spectrum /&bld;32f]colour token\n";
	_CPF_OSTR_PRINT(printf("%s\n", t_str));
	ASSERT_NO_THROW(c_printf( t_str));
}

TEST(Text_Colour_Formatting_Tokens, basic_full_colour_spectrum_background_with_attribute)
{
	auto t_str = "test full spectrum /&32b]colour token\n";
	_CPF_OSTR_PRINT(printf("%s\n", t_str));
	ASSERT_NO_THROW(c_printf( t_str));
}

TEST(Text_Colour_Formatting_Tokens, full_foreground_colour_spectrum_test)
{
	int count = 0;
	for(auto i(0u); i < 256; ++i )
	{
		std::stringstream ss;
		ss << i;
		auto frmt = std::string("/&").append(ss.str()).append("f]").append("%s").append("/!]\t").c_str();
		ASSERT_NO_THROW(c_printf( frmt, ss.str().c_str()));
	    if((++count % 4) == 0)
	    {
	        printf("\n");
	    }
	}
}

TEST(Text_Colour_Formatting_Tokens, full_background_colour_spectrum_test)
{
	int count = 0;
	for(auto i(0u); i < 256; ++i )
	{
		std::stringstream ss;
		ss << i;
		auto frmt = std::string("/&").append(ss.str()).append("b]").append("%s").append("/!]\t").c_str();
		ASSERT_NO_THROW(c_printf( frmt, ss.str().c_str()));
	    if((++count % 4) == 0)
	    {
	        printf("\n");
	    }
	}
}

TEST(Text_Colour_Formatting_Tokens, full_fg_bg_colour_spectrum_test)
{
	int count = 0;
	for(auto i(0u); i < 256; ++i )
	{
		for(auto j(0u); j < 256; ++j )
		{
			std::stringstream ss;
			ss << i << "f;" << j << "b";
			auto frmt = std::string("/&").append(ss.str()).append("]").append("%s").append("/!]\t").c_str();
			ASSERT_NO_THROW(c_printf( frmt, ss.str().c_str()));
		    if((++count % 8) == 0)
		    {
		        printf("\n");
		    }
		}
	}
}

#endif