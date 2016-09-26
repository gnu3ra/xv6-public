#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  sleep(1);
  printf(1, "ret: %d\n", gticks());
  printf(1, "tcount: %d\n", tcount());
  exit();
}
