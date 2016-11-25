#include "types.h"
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



/* basically a clone of ls, but recursive */ 
void recursion(char * path) {

  char buf[512], *p;
  int fd_tmp;
  struct dirent de;
  struct stat st;

  if((fd_tmp = open(path, 0)) < 0){
    printf(2, "cannot open %s\n", path);
    return;
  }

  int fd;
  fd = dup(fd_tmp);
  close(fd_tmp);

  
  if(fstat(fd, &st) < 0){
    printf(2, "cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", path, st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, " path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "cannot stat %s\n", buf);
        continue;
      }

      if(strcmp(de.name, "..")== 0)
        continue;
      if(strcmp(de.name, ".") == 0)
        continue;
      
      printf(1, "%s %d %d %d\n", path, st.type, st.ino, st.size);
      if(st.type == T_DIR) {
        //close(fd);
        
        recursion(filecat(path, de.name, '/'));
        //printf(1, "path: %s\n", path);
        //printf(1, "de.name: %s\n",de.name);
        //printf(1, "We should be recursing: %s\n",);
      }
    }
    break;
  }
//  close(fd);

}


int
main(int argc, char *argv[])
{
  recursion("./");
  exit();
}
