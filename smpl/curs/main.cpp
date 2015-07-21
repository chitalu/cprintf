#include <cprintf/cprintf.hpp>

int main(void) {
  //TODO: more sample needed here!
  //
  //clear console buffer first
  cprintf(L"$!");
  cprintf(L"Quux $4,3position change\n");
  cprintf(L"Foo $0,0`back to $gorigin\n");
  cprintf(L"Change position and colour $4,5.m* like this !!!\n");

  return 0;
}
