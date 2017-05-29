#include <cprintf/cprintf.hpp>

#include <cstdio>
#include <limits>
#include <cstdint>
#include <cinttypes>

int main(void) {

  /* 
	ref: http://www.cplusplus.com/reference/cstdio/printf/
  */
  cprintf("Characters: %c %c \n", 'a', 65);
  cprintf("Characters: /R%c %c \n", 'a', 65);

  cprintf("Decimals: %d %ld\n", 1977, 650000L);
  cprintf("$GDecimals$?: %d %ld\n", 1977, 650000L);

  cprintf("Preceding with blanks: %10d \n", 1977);
  cprintf("$BPreceding with blanks: %10d \n", 1977);

  cprintf("\nPreceding with zeros: %010d \n", 1977);
  cprintf("Preceding with zeros: $Y%010d \n", 1977);

  cprintf("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
  cprintf("Some different radices: %d %x %o $M%#x$W %#o \n", 100, 100, 100, 100, 100);

  cprintf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
  cprintf("floats: %4.2f $C%+.0e$? %E \n", 3.1416, 3.1416, 3.1416);

  cprintf("%s \n", "A string");
  cprintf("$W%s \n", "A string");

  cprintf("%-5s\n", "ab");
  cprintf("$r%-5s\n", "ab");

  /* 
	ref: http://en.cppreference.com/w/cpp/io/c/fprintf
  */
  cprintf("String: %s", "foo");
  cprintf("Strings:\n");

  const char *s = "Hello";
  cprintf("Characters:\t%c %%\n", 65);
  cprintf("\t.%10s.\n\t.%-10s.", s, s);

  cprintf("Integers\n");
  cprintf("Decimal:\t%i %d %.6i %i %.0i %+i %u\n", 1, 2, 3, 0, 0, 4, -1);
  cprintf("Hexadecimal:\t%x %x %X %#x\n", 5, 10, 10, 6);
  cprintf("Octal:\t%o %#o %#o\n", 10, 10, 4);

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
