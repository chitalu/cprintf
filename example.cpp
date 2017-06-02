#include <cprintf.h>

#include <cinttypes> // PRIu32

int main()
{
	cprintf_c_char_p_c_int32(1, "hello world %d\n", 1);

	cprintf("Characters: $R%c %c \n", 'a', 65);

	cprintf("$GDecimals$?: %d %ld\n", 1977, 650000L);

	cprintf("$BPreceding with blanks: %10d \n", 1977);

	cprintf("Preceding with zeros: $Y%010d \n", 1977);

	cprintf("Some different radices: %d %x %o $M%#x$W %#o \n", 100, 100, 100, 100, 100);

	cprintf("floats: %4.2f $C%+.0e$? %E \n", 3.1416, 3.1416, 3.1416);

	cprintf("$W%s \n", "A string");

	cprintf("$r%-5s\n", "ab");

	cprintf("String: $cR%s\n", "my string arg");

	cprintf("Str$Mi$?ngs:\n");

	const char *s = "Hello";
	cprintf("Char$cacters:\t%c %%\n", 65);
	cprintf("\t$Y.%10s$?.\n\t.%-10s.", s, s);

	cprintf("$YIntegers\n");
	cprintf("Decimal:\t$R%i $G%d $B%.6i $C%i $Y%.0i $M%+i $W%u\n", 1, 2, 3, 0, 0, 4, -1);
	cprintf("Hexadecimal:\t$R#%x $G#%x $B#%X $C#%#x\n", 5, 10, 10, 6);
	cprintf("$b#Octal:\t%o %#o %#o\n", 10, 10, 4);

	cprintf("Floating point\n");
	cprintf("Rounding:\t%f %.0f %.32f\n", 1.5, 1.5, 1.3);
	cprintf("Padding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
	cprintf("Scientific:\t%E %e\n", 1.5, 1.5);
	cprintf("Hexadecimal:\t%a %A\n", 1.5, 1.5);

#undef max
	std::uint32_t val = std::numeric_limits<std::uint32_t>::max();
	cprintf("Largest 32-bit value is %" PRIu32 " or %#" PRIx32 "\n", val, val);

	/*
	The following are not supported:

	cprintf("\t.%10s.\n\t.%-10s.\n\t.%*s.\n", s, s, 10, s);
	cprintf("Special values:\t0/0=%g 1/0=%g\n", 0. / 0, 1. / 0);
	cprintf("right-justified variable width: '%*c'\n", 5, 'x');
	*/

	return 0;
}