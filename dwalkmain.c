#include "types.h"
#include "user.h"
#include "dwalk.h"

int
main(int argc, char *argv[])
{
  struct unode* n =  dwalk("./");

  while(n->next != 0x0) {
    printf(1,"%d %d %d\n", n->inum, n->type, n->nlinks);
    n = n->next;
  }
  exit();
}
