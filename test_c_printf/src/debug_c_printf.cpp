#include "cpf_test_common.h"

TEST(Debug_print, basic_call)
{
	ASSERT_NO_THROW(c_printf_dbg("test string\n\n"));
}

TEST(Debug_print, basic_call_with_str_arg)
{
	ASSERT_NO_THROW(c_printf_dbg("testing string %s\n\n", "hello world"));
}

TEST(Debug_print, basic_call_with_colour)
{
	ASSERT_NO_THROW(c_printf_dbg("i am testing /c!]colour/!]\n\n"));

}

TEST(Debug_print, basic_call_with_str_arg_and_colour)
{
	ASSERT_NO_THROW(c_printf_dbg("testing string %s i am testing /y!]colour/!]\n\n", "hello world"));
}