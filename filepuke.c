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


void help(void) {
  printf(1, "type (0)\nmajor (1)\nminor (2)\nnlink (3)\nsize (4)\n");
}

void printdinode(struct dinode * in) {
  printf(1,"inode summary: \n");
  printf(1,"type: %d\nsize: %d\nlinks: %d\n",in->type, in->size, in->nlink);
}


void zerosingle(struct dinode * in, int index) {
  
}

int main(int argc, char ** argv) {

  if(argc < 2) {
    help();
    exit();
  }
  else {
    if(argc == 2)
      printdinode(openfile(argv[1]));
    else if(argc >= 3) {
      if(strcmp(argv[2], "c") == 0) {
        
      }
    }
  }
  exit();
  return 0;
}
