#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  sleep(100);
  printf(1, "[proccess1] Ticks from one second of sleep %d\n", gticks());
  printf(1, "[proccess1] Traps so far (including this call) %d\n", tcount());
  sleep(600);
  printf(1, "[proccess1] Ticks from six second of sleep %d\n", gticks());
  printf(1, "[proccess1] Traps so far (including this call) %d\n", tcount());

  if(fork() == 0) {
    sleep(100);
    printf(1, "[process2] Ticks from one second of sleep %d\n", gticks());
    printf(1, "[process2] Traps so far (including this call) %d\n", tcount());
    sleep(600);
    printf(1, "[process2] Ticks from six second of sleep %d\n", gticks());
    printf(1, "[process2] Traps so far (including this call) %d\n", tcount());
  }
  else {
    wait();
  }
  
  exit();
}
