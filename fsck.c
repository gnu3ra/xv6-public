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
  char isdir; 
};

struct lcount {
  uint inum;
  uint count;
};

void incrementfoundlist(int,struct lcount *, int, int *);


struct link * links;
struct lcount  * linkcounts;
int linkcountsize;
int linkssize; 
int checklinks(struct unode * first, struct unode * second) {
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
    if(first->type == T_DIR)
      i.isdir = 1;
    else
      i.isdir = 0;
    links[counter] = i;
    counter++;
    first = first->next;
  }

  linkssize = counter;
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
  //sizes should be the salt
  for(i=0;i<linkssize;i++) {
    uint compare = links[i].lt; 
    incrementfoundlist(compare,explinkcount,linkcountsize ,&incounter);
    
    /*
    if(links[i].isdir == 1) {
      int k;
      printf(1,"searching directory\n");
      //search the list for the parent and increltnt link count
      for(k=0;k<linkssize;k++) {
        if(links[i].lt == links[k].base) {
          printf(1, "found parent\n");
        }
      }
    }

    */
  }

  for(i=0;i<linkssize;i++) {
    printf(1, "%d ", linkcounts[i].count);
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
  
}


void incrementfoundlist(int compare,struct lcount * explinkcount, int size, int * incounter ) {
  int x;
  for(x=0;(x<size);x++) {
    if(compare == explinkcount[x].inum) {
      printf(1, "found one link before, incrementing\n");
      explinkcount[x].count++;
    }
    else {
      struct lcount c;
      c.count = 1;
      c.inum = compare; 
      explinkcount[*incounter] = c;
      (*incounter)++;
      break;
    }
  }  
}

int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();
  struct unode * dw = first;
  struct unode * iw = second;
  
  printf(1,"compare ret value %d\n", ucompare(first,second));

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
