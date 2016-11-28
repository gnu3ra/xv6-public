#include "types.h"
#include "compare.h"
#include "iwalk.h"
#include "stat.h"
#include "fcntl.h"
#include "param.h"
#include "user.h"
#include "fs.h"
#include "syscall.h"
#include "memlayout.h"


struct link {
  uint lt;
  uint base;
  int childinc;
  char isdir; 
};

struct lcount {
  uint inum;
  uint count;
  uint lt;
};


/* orphaned inode: an inode with no link pointing to it. 
 * detected by an inode being alocated, but with a link count of zero. 
 * fixed by creating tmp directory and linking from there. 
 */
#define ORPHAN 1

/* 
 * danlging links: a link with no inode at the end. Nothing to do but clear it. 
 */
#define DLINK 2

/*
 * link mismatch: the inode's link count is different from the measured one
 * in the directory tree. Better trust the tree and reset the inode
 */
#define LMISMATCH 3


struct fixme {
  int inum;
  int type;
};

void incrementfoundlist(struct link,struct lcount *, int, int *);


struct link * links;
struct lcount  * linkcounts;
int linkcountsize;
int linkssize;
int checklinks(struct unode * first, struct unode * second) {
  if(second->size != first->size) {
    printf(1, "Size mismatch: dwalk: %d iwalk: %d\n", first->size, second->size);
    //exit();
  }
  linkssize = first->size;
  linkcountsize = first->size;
  linkcounts = malloc(sizeof(struct lcount)* second->size);
  links = malloc(sizeof(struct link) * second->size);
  
  int root;
  int counter;
  root = 0;
  counter = 0;

  //walk over inodes and find link counts
  while(second->next != 0x0) {
    //printf(1, "loop 1\n");
    if(second->nlinks == 0) {
      root++;
    }
    if(root > 1)
      return -1;
    struct lcount l;
    l.count = second->nlinks;
    l.inum = second->inum;
    linkcounts[counter] = l;
    counter++;
    second = second->next;
    
  }

  linkcountsize = counter;
  counter = 0;

  //walk over directories and find what links to waht
  while(first->next != 0x0) {
    //printf(1, "loop 2\n");
    struct link i;
    i.base = first->tlinks;
    i.lt = first->nlinks;
    i.childinc = first->childinc;
    if(first->type == T_DIR)
      i.isdir = 1;
    else
      i.isdir = 0;
    links[counter] = i;
    counter++;
    first = first->next;
  }
  if(linkssize != linkcountsize) {
    printf(1, "computed size mismatch, linkssize: %d linkscountsize: %d\n", linkssize, linkcountsize);
    //exit();
  }
  return 0;
}

void reduce(struct lcount * real, struct lcount * fake) {
  int x;
  uint realresult = 0;
  uint fakeresult = 0;

  //sort both links by inode
  int i;
  for(x=0;x<linkssize-1;x++) {
    for(i=0;i<linkssize-x-1;i++) {
      if(real[i].inum > real[i+1].inum) {
        struct lcount tmp = real[i];
        real[i] = real[i+1];
        real[i+1] = tmp;
      }
      
      if(fake[i].inum > fake[i+1].inum) {
        struct lcount tmp = fake[i];
        fake[i] = fake[i+1];
        fake[i+1] = tmp;
      }
    }
  }

  
  for(x=0;x<linkssize;x++) {
    realresult += real[x].count;
    fakeresult += fake[x].count;
    if(fake[x].count != real[x].count) {
      printf(1,"FIXME: mismatched inode at [%d|%d] child: %d\n",real[x].inum, fake[x].inum, fake[x].lt);
    }
  }
//
  printf(1, "[reduce] real: %d fake: %d\n",realresult, fakeresult);
}

int writedir(char * path, struct dirent * entries, int size) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;


  mkdir(path);
  
  if((fd = diropen(path, O_RDWR)) < 0){
    printf(2, "cannot open %s\n", path);
    return -2;
  }
  
  if(fstat(fd, &st) < 0){
    printf(2, "cannot stat %s\n", path);
    close(fd);
    return -3;
  }
  
  
  switch(st.type){
  case T_FILE:
    return -1;
    break;
    
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, " path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    struct dirent * dirbuf = malloc(sizeof(struct dirent)*DIRSIZ);

    int counter = 0;
    // printf(1, "parent candidate %d\n", parentcandidate->inum);
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "cannot stat %s\n", buf);
        continue;
      }
      
      dirbuf[counter] = de;
      counter++;
    }
    close(fd);


    if((fd = diropen(path, O_RDWR)) < 0){
      printf(2, "cannot open %s\n", path);
      return -2;
    }
    
    if(fstat(fd, &st) < 0){
      printf(2, "cannot stat %s\n", path);
      close(fd);
      return -3;
    }

    
    int x;
    for(x=0;x<counter;x++) {
      printf(1,"[stage 1] writing %s\n", dirbuf[x].name);
      //do this once we know what is in the file
      write(fd, &(dirbuf[x]), sizeof(de));
    }

    for(x=0;x<size;x++) {
      printf(1,"[stage 2] writing %s\n", dirbuf[x].name);
      write(fd, &(entries[x]), sizeof(entries[x]));
    }
    break;
  }
   close(fd);
   return 0;
}

void processlinks(void) {
  int i;
  int linktotal = 0;
  for(i=0;i<linkcountsize;i++) {
    linktotal += linkcounts[i].count; 
  }

  
  struct lcount * explinkcount;
  explinkcount = malloc(sizeof(struct lcount) * linkcountsize);
  printf(1, "total links (iwalk) %d\n" , linktotal);
  int incounter;
  incounter = 0;
  //sizes should be the same
  for(i=0;i<linkssize;i++) {
    incrementfoundlist(links[i],explinkcount,linkcountsize ,&incounter);
  }
  printf(1,"real:\n");
  for(i=0;i<linkssize;i++) {
      printf(1, "%d ", linkcounts[i].inum);
  }
  printf(1,"\n");

  for(i=0;i<linkssize;i++) {
    printf(1, "%d ", linkcounts[i].count);
  }
  printf(1,"\nfake:\n");
  for(i=0;i<linkssize;i++) {
      printf(1, "%d ", explinkcount[i].inum);
  }
  printf(1,"\n");
  for(i=0;i<linkssize;i++) {
      printf(1, "%d ", explinkcount[i].count);
  }
  printf(1,"\n");
  for(i=0;i<linkssize;i++) {
    printf(1, "%d->%d ", links[i].base, links[i].lt);
  }
  printf(1,"\n");

  reduce(linkcounts, explinkcount);
  
}


void incrementfoundlist(struct link comp ,struct lcount * explinkcount, int size, int * incounter ) {
  int x;
  for(x=0;(x<size);x++) {
    if(comp.lt == explinkcount[x].inum) {
      //     printf(1, "found one link before, incrementing\n");
      explinkcount[x].count++;
    }
    else {
      struct lcount c;
      c.count = 1;
      c.inum = comp.lt;
      c.lt = comp.base;
      if(comp.isdir == 1) {
        printf(1, "Adding childcount %d from link [%d|%d]\n" ,comp.childinc, comp.base, comp.lt);
        c.count += comp.childinc;
      }
      explinkcount[*incounter] = c;
      (*incounter)++;
      break;
    }
  }  
}


char * itoa(int integer) {
  char * ascii = malloc(sizeof(char)*20); //scaaaary
  int temp,count=0,i,cnd=0;
  if(integer>>31)
  {
    integer=~integer+1;
    for(temp=integer;temp!=0;temp/=10,count++);
    ascii[0]=0x2D;
    count++;
    cnd=1;
  }
  else
    for(temp=integer;temp!=0;temp/=10,count++);
  for(i=count-1,temp=integer;i>=cnd;i--)
  {

    ascii[i]=(temp%10)+0x30;
    temp/=10;
  }

  return ascii;
}


/* this would be better if lists were sorted, but no time and 
 * they already are on an unused filesystem 
 */ 
void repairorphans(struct unode * dwalkin, struct unode * iwalkin) {
  printf(1,"check 1\n");
  struct unode * dwalk = dwalkin->first;
  struct unode * iwalk = iwalkin->first;
  struct unode * dwalkreset = dwalkin->first;
  int dsize = dwalk->size;
  int isize = iwalk->size;
  printf(1,"check 2 (size %d %d)\n", dwalk->size, iwalk->size);
  struct dirent * inods = malloc(sizeof(struct dirent) * (isize -  dsize + 1));
  int counter = 0;
  int engage = 1;
  while(iwalk != 0x0) {
    engage = 1;
    while(dwalk != 0x0){
      if(dwalk->inum == iwalk->inum) {
        engage = 0;
      }
      dwalk = dwalk->next;
    }
    if(engage==1) {
      printf(1, "selecting inode to repair\n");
      struct dirent d;
      d.inum = iwalk->inum;
      strcpy( d.name, itoa(iwalk->inum));
      inods[counter] = d;
      counter++;
    }
    dwalk = dwalkreset;
    iwalk = iwalk->next;
  }

  printf(1,"relinking %d inodes\n", counter);
  writedir("/RECOVER", inods,counter );
  return;
}

int main(void) {

//  struct dirent t[1];
//  t[0].inum = 4;
//  t[0].name[0] = 'y';
//  t[0].name[1] = '\0';
//  writedir("/gooo" ,t, 1);
  
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();
  struct unode * dw = first;
  struct unode * iw = second;

  int cmpr = ucompare(first,second);
  printf(1,"compare ret value %d\n",cmpr );

  if(cmpr == -1) {
    if(first->size < second->size) {
      printf(1, "orphaned inodes detected: cleaning\n");
      repairorphans(first, second);
    }
  }
  
  struct unode * dw_check;
  struct unode * iw_check;
  iw_check = iw;
  dw_check = dw;

  printf(1, "Starting link check\n");
  if(checklinks(dw_check, iw_check)==0) {
    processlinks();
  }
  
  exit();
}
