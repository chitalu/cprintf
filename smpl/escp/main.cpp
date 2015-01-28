#include <cprintf/cprintf.hpp>

int main(void)
{
	bool exec_ok = true;

	try
	{
		cprintf("bar$r`r\n");

		cprintf("bar $r*`red\n");

		cprintf("$b`blue\n");

		cprintf("$b*`blue\n");

		cprintf("$y*#`bright yellow background\n");

		cprintf("$r*#`bright red background\n");

		cprintf("`$? `$r `$g `$b `$y `$m `$c `$w `$rg `$gb* `$b*y `$y*m* `$m ");
	}
	catch (cpf::type::except &e)
	{
		fwprintf(stderr, L"cprintf sample failed\n%s\n", e.msg());
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}