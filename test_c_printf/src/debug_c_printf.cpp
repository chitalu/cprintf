#include "cpf_test_common.h"

TEST(Debug_print, basic_call)
{
	c_printf_dbg("test string\n\n");
}

TEST(Debug_print, basic_call_with_str_arg)
{
	c_printf_dbg("testing string %s\n\n", "hello world");
}

TEST(Debug_print, basic_call_with_colour)
{
	c_printf_dbg("i am testing /c*]colour/*]\n\n");

}

TEST(Debug_print, basic_call_with_str_arg_and_colour)
{
	c_printf_dbg("testing string %s i am testing /y*]colour/*]\n\n", "hello world");
}