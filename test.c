#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL 0x0


int
main(int argc, char *argv[])
{
  printf(1,"If you see \"no segfault\", then segfault has failed\n");
  int * x;
  x =(unsigned long) NULL;
  int y  = *x;
  printf(1,"pointer is %p\n value is %d \n",x, y);

  printf(1,"hello\n");
  exit();
//  return 0;
}
