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
  uint ito;
  uint ifrom;
};

struct lcount {
  uint inum;
  uint count;
};

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
    i.ifrom = first->tlinks;
    i.ito = first->nlinks;
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

  int x;
  //sizes should be the same
  for(i=0;i<linkssize;i++) {
    int tmpexp = 0;
    for(x=0;x<linkssize;x++) {
      if(links[x].ifrom == links[i].ito)
        tmpexp++;
    }
    struct lcount n;
    n.count = tmpexp;
    n.inum = links[i].ito;
    explinkcount[i] = n;
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
  checklinks(dw_check, iw_check);
  
  exit();
}
