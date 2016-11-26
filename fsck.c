#include "types.h"
#include "compare.h"
#include "iwalk.h"
#include "stat.h"
#include "fcntl.h"
#include "param.h"
#include "user.h"
#include "fs.h"
#include "syscall.h"
#include "memlayout.h"



int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();

  printf(1,"compare ret value %d\n", ucompare(first,second));

  
  exit();
}
