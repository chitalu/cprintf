#include "cprintf/cprintf.h"

int main(void)
{
	bool exec_ok = true;

	try
	{
		cprintf("foo $r`red\n");
		cprintf("bar $r*`red\n");
		cprintf("quux$g green\n");
		cprintf("$b`blue\n");
		cprintf("$b*`blue\n");

		cprintf("foo $r%s\n", "red");
		cprintf("bar $r*%s\n", "red");
		cprintf("quux$g %s\n", "green");
		cprintf("$b%s\n", "blue");
		cprintf("$b*%s\n", "blue");

		cprintf("foo$r.y yellow\n");
		cprintf("bar$r.y* yellow\n");
		cprintf("qux$b.r red\n");
		cprintf("quux$b.r* red\n");

		cprintf("setting background\n");
		cprintf("$#ydim yellow\n");
		cprintf("$#y*`bright yellow\n");
		cprintf("$#r*`bright red\n");
		cprintf("$#gdim green\nand so on...\n");

		/*cprintf("$? $r $g $b $y $m $c $w $rg $gb* $b*y $y*m* \$m ");

		cprintf("$bld $7,4 $g.bld $rvs.r*");*/
	}
	catch (_cpf_type::error &e)
	{
		std::cout << e.what() << std::endl;
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}