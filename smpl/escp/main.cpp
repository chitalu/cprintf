#include <cprintf/cprintf.hpp>

int main(void) {
  cprintf(L"bar$r`r\n");

  cprintf(L"bar $r*`red\n");

  cprintf(L"$b`blue\n");

  cprintf(L"$b*`blue\n");

  cprintf(L"$y*#`bright yellow background\n");

  cprintf(L"$r*#`bright red background\n");

  cprintf(L"`$? `$r `$g `$b `$y `$m `$c `$w `$rg `$gb* `$b*y `$y*m* `$m \n");
  return EXIT_SUCCESS;
}
