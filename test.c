#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  
  int * x;
  x =(unsigned long) 0x0;
  int y  = *x;
  printf(1,"pointer is %p\n value is %d \n",x, y);

  printf(1,"hello\n");
  sleep(100);
  exit();
//  return 0;
}
