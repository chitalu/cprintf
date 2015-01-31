#include <cprintf/cprintf.hpp>

int main(void)
{	
	cprintf("Quux $4,3position change");
	cprintf("Foo $0,0`back to $gorigin");
	cprintf("Change position and colour $4,5.m* like this !!!\n");

	return 0;
}
