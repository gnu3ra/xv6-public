#include "types.h"
#include "user.h"
#include "compare.h"
#include "iwalk.h"

int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();

  printf(1,"compare ret value %d\n", ucompare(first,second));
  exit();
}
