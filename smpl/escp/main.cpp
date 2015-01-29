#include <cprintf/cprintf.hpp>

int main(void)
{
	auto ret = cprintf("floyd\n");

	ret = cprintf("hello %d %d\n", ret.f, ret.c);
	
	/*cprintf("bar$r`r\n");

	cprintf("bar $r*`red\n");

	cprintf("$b`blue\n");

	cprintf("$b*`blue\n");

	cprintf("$y*#`bright yellow background\n");

	cprintf("$r*#`bright red background\n");

	cprintf("`$? `$r `$g `$b `$y `$m `$c `$w `$rg `$gb* `$b*y `$y*m* `$m ");*/

	return EXIT_SUCCESS;
}