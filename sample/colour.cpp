#include "cprintf/cprintf.h"

int main(void)
{
	bool exec_ok = true;

	try
	{
		cprintf("quux$g green\n");

		cprintf("foo$r.y yellow\n");
		cprintf("bar$r.y* yellow\n");
		cprintf("qux$b.r red\n");
		cprintf("quux$b.r* red\n");

		cprintf("setting background\n");
		cprintf("$#ydim yellow\n");
		cprintf("$#gdim green\nand so on...\n");

		/*cprintf("$? $r $g $b $y $m $c $w $rg $gb* $b*y $y*m* \$m ");

		cprintf("$bld $7,4 $g.bld $rvs.r*");*/
	}
	catch (_cpf_type::error &e)
	{
		fprintf(stderr, "cprintf sample failed\n%s\n", e.what());
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}