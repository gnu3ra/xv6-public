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
  for(i=0;i<30;i++) {
    struct inode * test =  malloc(sizeof(struct inode));
    getinode(test, 1, i);
    printf(1,"flags: %d inum: %d nlink: %d\n", test->flags, test->inum, test->nlink );
    free(test);
  }
  exit();
}
