#include "types.h"

#define BUFLEN 5

#ifndef DWALKH
#define DWALKH

struct unode {
  uint inum;
  uint type;
  uint tlinks;
  uint nlinks;
  uint childinc;
  int size;
  int flags;
  struct unode * first;
  struct unode * parent;
  struct unode * next;
}; 


char * filecat(char*, char*, char);
struct unode * dwalk(char*);

#endif
