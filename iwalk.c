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

  int ic;
  ic = icount();
  printf(1, "read superblock: %d inodes\n",ic);
  int i;
  for(i=0;i<ic;i++) {
  
    struct inode * test = malloc(sizeof(struct inode));
    
    if(getinode(test,0,i) < 0) {
      printf(1, "error in kernel. Plz call 555-CALL-HELP\n");
      free(test);
      continue;
    }
    else {
      printf(1, "found inode!~ inum: %d type: %d\n", test->inum, test->type);
      free(test);
    }
  }
  exit();
}
