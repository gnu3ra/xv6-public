#include "types.h"
#include "param.h"
#include "user.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "stat.h"
#include "buf.h"
#include "fcntl.h"
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



void zerodir(char * path, int offset) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct dirent wr;
  struct stat st;


  wr.inum = 0;
  int i;
  for(i=0;i<14;i++) {
    wr.name[i] = '\0';
  }
  
  if((fd = diropen(path, O_RDWR )) < 0){
    printf(2, "cannot open %s\n", path);
    return ;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  
  switch(st.type){
  case T_FILE:
    //file stuff
    break;
    
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, " path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    int counter =0;
    do{
      if(counter == offset) {
        printf(1, "writing to de entry [%s]\n",de.name );
        
        write(fd, &wr, sizeof(wr));
        break;
      }
      counter ++;
    }while (read(fd, &de, sizeof(de)) == sizeof(de));
    close(fd);
    break;
  }
  
}


void help(void) {
  printf(1, "type (0)\nmajor (1)\nminor (2)\nnlink (3)\nsize (4)\n");
}

void printdinode(struct dinode * in) {
  printf(1,"inode summary: \n");
  printf(1,"type: %d\nsize: %d\nlinks: %d\n",in->type, in->size, in->nlink);
}


void zerosingle(struct dinode * in, int index) {
  switch(index) {
  case 0:
    in->type = 0;
    break;
  case 1:
    in->major = 0;
    break;
  case 2:
    in->minor = 0;
    break;
  case 3:
    in->nlink = 0;
    break;
  case 4:
    in->size = 0;
    break;
  default:
    memset((void*)in, 0, sizeof(struct dinode));
  }
}

int main(int argc, char ** argv) {
  if(argc == 3) {
    zerodir(argv[1], atoi(argv[2]));
  }

  exit();
  return 0;
}
