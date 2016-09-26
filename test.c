#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  sleep(1);
  printf(1, "Ticks from one second of sleep %d\n", gticks());
  printf(1, "Traps so far (including this call) %d\n", tcount());
  sleep(6);
  printf(1, "Ticks from six second of sleep %d\n", gticks());
  printf(1, "Traps so far (including this call) %d\n", tcount());
  
  exit();
}
