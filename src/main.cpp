#include "c_printf.h"

int main(int argc, char const *argv[])
{
	/* test code */
	
	c_printf(stdout, "test foo %d ${rb} %f ${b} %s ${g} done.", 10, 10.0, "hello world!");
	
	return 0;
}