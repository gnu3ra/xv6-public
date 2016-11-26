#include "types.h"

#define BUFLEN 5

#ifndef DWALKH
#define DWALKH

struct unode {
  uint inum;
  uint type;
  uint nlinks;
  uint linkto;
  struct unode * first;
  struct unode * next;
}; 


char * filecat(char*, char*, char);
struct unode * dwalk(char*);

#endif
