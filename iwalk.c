#include "types.h"
#include "param.h"
#include "user.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"
#include "syscall.h"
#include "memlayout.h"
/*
 * Recursive ls-like directory walker command
 */ 


char * filecat(char * one, char * two, char sep) {
  int onelen;
  int twolen;
  onelen = strlen(one);
  twolen = strlen(two);
  char * result;
  result  = malloc((onelen + 1 + twolen) * sizeof(char));
  int i;
  for(i=0;i<onelen;i++) {
    result[i] = one[i];
  }
  result[onelen] = sep;
  for(i=onelen+1;i<(onelen + 1 + twolen);i++) {
    result[i] = two[i-(onelen + 1)];
  }
  return result; 
}

int
main(int argc, char *argv[])
{
  //recursion("./");

  printf(1, "read superblock: %d inodes\n",icount() );
  struct inode * test = 0x0;
  getinode(test, 0, 2);
  printf(1, "getinode returns %d\n",0 );
  exit();
}
