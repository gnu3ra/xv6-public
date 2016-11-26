#include "types.h"
#include "param.h"
#include "user.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"
#include "syscall.h"
#include "memlayout.h"
#include "file.h"

/*
 * Recursive ls-like directory walker command
 */ 

int
main(int argc, char *argv[])
{
  uint i;
  for(i=1;i<icount();i++) {
    struct dinode * test =  malloc(sizeof(struct dinode));
    getinode(test, 1, i);
    if(test->type != 0)
      printf(1,"type: %d inum: %d nlink: %d\n", test->type, i , test->nlink);
    free(test);
  }
  exit();
}
