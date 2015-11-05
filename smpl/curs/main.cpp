#include <cprintf/cprintf.hpp>

int main(void) {
  //TODO: more sample needed here!
  //
  //clear console buffer first
  cprintf("$!");
  cprintf("Quux $4,3position change\n");
  cprintf("Foo $0,0`back to $gorigin\n");
  cprintf("Change position and colour $4,5.m* like this !!!\n");

  return 0;
}
