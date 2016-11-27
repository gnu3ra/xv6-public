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



struct link * links;
uint  * linkcounts;

int checklinks(struct unode * first, struct unode * second) {
  linkcounts = malloc(sizeof(uint)* second->size);
  links = malloc(sizeof(struct link) * second->size);
  
  int root;
  int counter;
  root = 0;
  counter = 0;
  while(first->next != 0x0) {
    //printf(1, "loop 1\n");
    if(first->nlinks == 0) {
      root++;
    }
    if(root > 1)
      return -1;
    
    linkcounts[counter] = first->nlinks;
    counter++;
    first = first->next;
    
  }
  counter = 0;
  while(second->next != 0x0) {
    //printf(1, "loop 2\n");
    struct link i;
    i.ifrom = second->tlinks;
    i.ito = second->nlinks;
    links[counter] = i;
    counter++;
    second = second->next;
  }
  return 0;
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
