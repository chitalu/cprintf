# cprintf #

### Examples: ###

```cpp
cprintf(stdout, "Characters: $R%c %c \n", 'a', 65);
cprintf(stderr, "$GDecimals$?: %d %ld\n", 1977, 650000L);
cprintf("$BPreceding with blanks: %10d \n", 1977);
cprintf(L"Preceding with zeros: $Y%010d \n", 1977);
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
cprintf("$cFloating point\n");
cprintf("Ro$yunding:\t%f $M%.0f $W%.32f\n", 1.5, 1.5, 1.3);
cprintf("$WPadding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
cprintf("Scientific:$R\t%E $W%e\n", 1.5, 1.5);
cprintf("$BHexadecimal:\t%a %A\n", 1.5, 1.5);
std::uint32_t val = std::numeric_limits<std::uint32_t>::max();
cprintf("Largest 32-bit value is $Y%" PRIu32 "$? or $C%#" PRIx32 "\n", val,
  val);
```

### Examples Output: ###

![cprintf-output-linux](https://user-images.githubusercontent.com/7461679/27014588-afa37e3a-4ef3-11e7-84ad-00bed11d5610.jpg)

## Project Depedencies: ##

**Windows:**
* Microsoft (R) C/C++ Optimizing Compiler Version 18.00.21005.1 (Visual Studio 2013)
 
**Linux**
* GNU Compiler Collection (GCC) 4.8 or Clang 3.4
