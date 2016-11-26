#include "types.h"
#include "param.h"
#include "user.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "stat.h"
#include "buf.h"
#include "syscall.h"
#include "memlayout.h"
#include "file.h"


struct dinode *  openfile(char * path) {
  int fd;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "cannot open %s\n", path);
    return 0x0;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "cannot stat %s\n", path);
    close(fd);
    return 0x0;
  }

  struct dinode * test =  malloc(sizeof(struct dinode));
  getinode(test, 1, st.ino);
  return test;
}


int main(int argc, char ** argv) {

  return 0;
}
