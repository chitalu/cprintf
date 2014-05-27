#include "cpf_test_common.h"

TEST(Format_specifiers, std_formatting)
{
	/*

	these are samples from http://www.cplusplus.com/reference/cstdio/printf/
	*/
	try
	{
		c_printf("Characters:/y*] %c %c \n", 'a', 65);
		c_printf("Decimals: %d %ld\n", 1977, 650000L);
		c_printf("Preceding with blanks: %10d \n", 1977);
		c_printf("Preceding with zeros: %010d \n", 1977);
		c_printf("Some different radices:/c*] %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
		c_printf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
		c_printf("%s \n", "A string");
		c_printf("%-5s", "ab");
	}
	catch (_cpf_type::error& e)
	{
		printf("%s", e.what());
		e;
	}
}

TEST(Format_specifiers, unsupported_std_formatting)
{
	try
	{
		/*
			this is not supported as a result of method of implementation i.e
			c_printf is implementated in such a way that only one argument may be 
			processed at a time.
		*/
		c_printf("Width trick: %*d \n", 5, 10);
	}
	catch (_cpf_type::error& e)
	{
		printf("%s", e.what());
		e;
	}
}
