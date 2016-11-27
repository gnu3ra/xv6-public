#include "dwalk.h"
#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


/*
 * Recursive ls-like directory walker command
 */ 


static struct unode * recursion(char*, struct unode*, int *, struct unode*);

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


struct unode * dwalk(char* path) {
  struct unode * n = malloc(sizeof(struct unode));
  n->first = n;
  int size = 0; 
  recursion(path, n,&size, n);
  n = n->first;
  n->size = size;
  return n;
}


/* basically a clone of ls, but recursive */ 
static struct unode * recursion(char * path, struct unode * nodelist, int * size, struct unode * parentcandidate) {

  char buf[512], *p;
  int fd_tmp;
  struct dirent de;
  struct stat st;

  if((fd_tmp = open(path, 0)) < 0){
    printf(2, "cannot open %s\n", path);
    return 0x0;
  }

  int fd;
  fd = dup(fd_tmp);
  close(fd_tmp);

  
  if(fstat(fd, &st) < 0){
    printf(2, "cannot stat %s\n", path);
    close(fd);
    return 0x0;
  }

  switch(st.type){
  case T_FILE:
    printf(1,"This shouldn't be printed\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, " path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    if((*size)==0) {
      nodelist->type = st.type;
      nodelist->inum = st.ino;
      nodelist->tlinks = st.ino;
      nodelist->nlinks = 1;
      nodelist->next = malloc(sizeof(struct unode));
      nodelist->next->parent = nodelist;
      nodelist->childinc = 0;
      nodelist = nodelist->next;
      (*size)++;
    }
    uint dinum = st.ino;
    int dot = 0;
    int dotdot = 0;
    int numdirs = 0;
    printf(1, "parent candidate %d\n", parentcandidate->inum);
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "cannot stat %s\n", buf);
        continue;
      }
      char * chname;
      chname = filecat(path, de.name, '/');
      printf(1, "crawling unfiltered %s\n" ,chname);
      
      if(strcmp(de.name, "..")== 0) {
        dotdot++;
        continue;
      }
      if(strcmp(de.name, ".") == 0) {
        dot++;
        continue;
      }
      nodelist->type = st.type;
      nodelist->inum =   st.ino;
      printf(1, "crawling %s inum: %d\n" ,chname, st.ino);
      nodelist->nlinks = de.inum;
      nodelist->tlinks = dinum;
      nodelist->next = malloc(sizeof(struct unode));
      nodelist->parent = parentcandidate ;
      struct unode * tmp = nodelist;
      nodelist = nodelist->next;
      (*size)++;
      
      if(st.type == T_DIR) {
        
        printf(1,"incing %s inum: %d", de.name , nodelist->parent->inum);
        parentcandidate->childinc++;
        numdirs++;
        nodelist =  recursion(chname, nodelist,size,tmp );
        
      
      }
    }
    close(fd);
    break;
  }

  return nodelist;
//  close(fd);

}
