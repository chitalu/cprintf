#include "cpf_test_common.h"

TEST(Debug_c_printf, basic_call)
{
	ASSERT_NO_THROW(debug_c_printf("test string\n\n"));
}

TEST(Debug_c_printf, basic_call_with_str_arg)
{
	ASSERT_NO_THROW(debug_c_printf("testing string %s\n\n", "hello world"));
}

TEST(Debug_c_printf, basic_call_with_colour)
{
	ASSERT_NO_THROW(debug_c_printf("i am testing /c!]colour/!]\n\n"));

}

TEST(Debug_c_printf, basic_call_with_str_arg_and_colour)
{
	ASSERT_NO_THROW(debug_c_printf("testing string %s i am testing /y!]colour/!]\n\n", "hello world"));
}