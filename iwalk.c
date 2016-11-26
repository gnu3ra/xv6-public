#include "dwalk.h"
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
struct unode * iwalk(void) {
  struct unode * out = malloc(sizeof(struct unode));
  struct unode * ret = out;
  out->first = ret;
  uint i;
  for(i=1;i<icount();i++) {
    struct dinode * test =  malloc(sizeof(struct dinode));
    getinode(test, 1, i);
    if(test->type != 0) {
      //   printf(1, "count %d\n",i);
      out->type = (uint) test->type;
      out->inum =  i;
      out->next = malloc(sizeof(struct unode));
      out = out->next;
    }
    free(test);
  }
  return ret;
}

int
main(int argc, char *argv[])
{
  struct unode* n =  iwalk();

  while(n->next != 0x0) {
    printf(1,"%d %d\n", n->inum, n->type);
    n = n->next;
  }
  exit();
}
