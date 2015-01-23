#include <cprintf/cprintf.hpp>

int main(void)
{
	bool exec_ok = true;
	
	try
	{
		cprintf("Quux $4,3position change");
		cprintf("Foo $0,0`back to $gorigin");
		cprintf("Change position and colour $4,5.m* like this !!!\n");

        //cprintf_s("test\n");
	}
	catch (cpf::type::except &e)
	{
		fwprintf(stderr, L"cprintf sample failed\n%s\n", e.msg());
		exec_ok = false;;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
