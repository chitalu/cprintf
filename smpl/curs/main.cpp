#include <cprintf/cprintf.h>

int main(void)
{
	bool exec_ok = true;
	
	try
	{
		cprintf("Quux $4,3position change");
		cprintf("Foo $0,0`back to $gorigin");
		cprintf("Change position and colour $4,5.m* like this !!!\n");
	}
	catch (cpf::type::except &e)
	{
		fprintf(stderr, "cprintf sample failed\n%s\n", e.msg());
		exec_ok = false;;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}