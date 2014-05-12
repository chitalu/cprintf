#include "cpf_test_common.h"


TEST(Debug_c_printf, basic_call)
{
	TEST_PERIMETER_EMPLACE_BEGIN
	{
		ASSERT_NO_THROW(debug_c_printf("test string"));
	}
	TEST_PERIMETER_EMPLACE_END
}

TEST(Debug_c_printf, basic_call_with_str_arg)
{
	TEST_PERIMETER_EMPLACE_BEGIN
	{
		ASSERT_NO_THROW(debug_c_printf("testing string %s", "hello world"));
	}
	TEST_PERIMETER_EMPLACE_END
}

TEST(Debug_c_printf, basic_call_with_colour)
{
	TEST_PERIMETER_EMPLACE_BEGIN
	{
		ASSERT_NO_THROW(debug_c_printf("i am testing /c!]colour/!]"));
	}
		TEST_PERIMETER_EMPLACE_END
}

TEST(Debug_c_printf, basic_call_with_str_arg_and_colour)
{
	TEST_PERIMETER_EMPLACE_BEGIN
	{
		ASSERT_NO_THROW(debug_c_printf("testing string %s i am testing /y!]colour/!]", "hello world"));
	}
	TEST_PERIMETER_EMPLACE_END
}