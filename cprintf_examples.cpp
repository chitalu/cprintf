#include <cprintf/cprintf.h>
#include <cinttypes> // PRIu32

int main(void) {
  cprintf(stdout, "Characters: $R%c %c \n", 'a', 65);
  cprintf(stderr, "$GDecimals$?: %d %ld\n", 1977, 650000L);
  cprintf("$BPreceding with blanks: %10d \n", 1977);
  cprintf("Preceding with zeros: $Y%010d \n", 1977);
  cprintf("Some different radices: %d %x %o $M%#x$W %#o \n", 100, 100, 100, 100,
          100);
  cprintf("floats: %4.2f $C%+.0e$? %E \n", 3.1416, 3.1416, 3.1416);
  cprintf("$W%s \n", "A string");
  cprintf("$r%-5s\n", "ab");
  cprintf("String: $cR%s\n", "my string arg");
  cprintf("Str$Mi$?ngs:\n");
  const char *s = "Hello";
  cprintf("Char$cacters:\t%c %%\n", 65);
  cprintf("\t$Y.%10s$?.\n\t.%-10s.", s, s);
  cprintf("$YIntegers\n");
  cprintf("Decimal:\t$R%i $G%d $B%.6i $C%i $Y%.0i $M%+i $W%u\n", 1, 2, 3, 0, 0,
          4, -1);
  cprintf("Hexadecimal:\t$R#%x $G#%x $B#%X $C#%#x\n", 5, 10, 10, 6);
  cprintf("$b#Octal:\t%o %#o %#o\n", 10, 10, 4);
  cprintf("$cFloating point\n");
  cprintf("Ro$yunding:\t%f $M%.0f $W%.32f\n", 1.5, 1.5, 1.3);
  cprintf("$WPadding:\t%05.2f %.2f %5.2f\n", 1.5, 1.5, 1.5);
  cprintf("Scientific:$R\t%E $W%e\n", 1.5, 1.5);
  cprintf("$BHexadecimal:\t%a %A\n", 1.5, 1.5);
#ifdef max
#undef max
#endif
  std::uint32_t val = std::numeric_limits<std::uint32_t>::max();
  cprintf("Largest 32-bit value is $Y%" PRIu32 "$? or $C%#" PRIx32 "\n", val,
          val);
#if 0
#error "Unsupported argument combinations"
	cprintf("\t.%10s.\n\t.%-10s.\n\t.%*s.\n", s, s, 10, s);
	cprintf("Special values:\t0/0=%g 1/0=%g\n", 0. / 0, 1. / 0);
	cprintf("right-justified variable width: '%*c'\n", 5, 'x');
#endif
  cprintf("std::string: %s\n", std::string("hello"));
  cprintf_s("string literal API\n");
#if 0
#error "Unsupported argument combinations"
	cprintf_s(s);
#endif
  cprintf("bar $rw`Red on white\n");
  cprintf("bar $r#ed\n");
  cprintf("bar $r#ed\n");
  cprintf("bar $r`#ed\n");
  cprintf("quux$r.b blue\n");
  cprintf("quux$B.r red\n");
  cprintf("quux$B.R red\n");
  cprintf("quux$r.B blue\n");
#ifdef CPF_WINDOWS_BUILD
  cprintf("note that $bld this text is blue\n");
#else // (0 - 255)
  cprintf("quux $128f xterm bitmap colour foreground\n");
  cprintf("quux $24b xterm bitmap colour background\n");
  cprintf("quux $128f.94b xterm bitmap colour foreground & background\n");
  cprintf("quux $128f$64b xterm bitmap colour foreground & background\n");
  cprintf("quux $64& xterm bitmap colour block\n");
  cprintf("note that $bld this text is bold\n");
  cprintf("note that $b`ld this text is blue\n");
  cprintf("drive forward and $rvs reverse $?rvsand the back to normal\n");
#endif
  std::vector<std::string> xplat_tokens = {
      "r",  "g",  "b",  "y",  "m",  "c",  "w",  "r#", "g#", "b#", "y#", "m#",
      "c#", "w#", "R",  "G",  "B",  "Y",  "M",  "C",  "W",  "R#", "G#", "B#",
      "Y#", "M#", "C#", "W#", "rr", "rb", "rg", "ry", "rm", "rc", "rw", "gg",
      "gb", "gr", "gy", "gm", "gc", "gw", "bb", "br", "bg", "by", "bm", "bc",
      "bw", "yy", "yb", "yg", "yr", "ym", "yc", "yw", "mm", "mr", "mg", "my",
      "mb", "mc", "mw", "cc", "cr", "cg", "cy", "cm", "cb", "cw", "ww", "wr",
      "wg", "wy", "wm", "wc", "wb", "Rr", "Rb", "Rg", "Ry", "Rm", "Rc", "Rw",
      "Gg", "Gb", "Gr", "Gy", "Gm", "Gc", "Gw", "Bb", "Br", "Bg", "By", "Bm",
      "Bc", "Bw", "Yy", "Yb", "Yg", "Yr", "Ym", "Yc", "Yw", "Mm", "Mr", "Mg",
      "My", "Mb", "Mc", "Mw", "Cc", "Cr", "Cg", "Cy", "Cm", "Cb", "Cw", "Ww",
      "Wr", "Wg", "Wy", "Wm", "Wc", "Wb", "rR", "rB", "rG", "rY", "rM", "rC",
      "rW", "gG", "gB", "gR", "gY", "gM", "gC", "gW", "bB", "bR", "bG", "bY",
      "bM", "bC", "bW", "yY", "yB", "yG", "yR", "yM", "yC", "yW", "mM", "mR",
      "mG", "mY", "mB", "mC", "mW", "cC", "cR", "cG", "cY", "cM", "cB", "cW",
      "wW", "wR", "wG", "wY", "wM", "wC", "wB", "RR", "RB", "RG", "RY", "RM",
      "RC", "RW", "GG", "GB", "GR", "GY", "GM", "GC", "GW", "BB", "BR", "BG",
      "BY", "BM", "BC", "BW", "YY", "YB", "YG", "YR", "YM", "YC", "YW", "MM",
      "MR", "MG", "MY", "MB", "MC", "MW", "CC", "CR", "CG", "CY", "CM", "CB",
      "CW", "WW", "WR", "WG", "WY", "WM", "WC", "WB"};
  for (std::vector<std::string>::const_iterator tokenIter =
           xplat_tokens.cbegin();
       tokenIter != xplat_tokens.cend(); ++tokenIter) {
    const std::string format = ("$" + (*tokenIter) + "%s" + "$?\t");
    cprintf(format, (*tokenIter));
    std::size_t i = std::distance(xplat_tokens.cbegin(), tokenIter) + 1;
    if ((i % 7) == 0) 
      cprintf("%s", i == xplat_tokens.size() - 1 ? "\n\n" : "\n");
  }
  cprintf("`$? `$r `$g `$b `$y `$m `$c `$w `$rg `$gB `$By `$YM `$m ...\n");
  cprintf("$? $r`r $g`g $b`b $y`y $m`m $c`c $w`w $rg`rg $gB`gB $By`By $YM`YM "
          "$m`m ...\n");
  return 0;
}
