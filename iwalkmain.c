#include "types.h"
#include "user.h"
#include "iwalk.h"

int
main(int argc, char *argv[])
{
  struct unode* n =  iwalk();

  while(n->next != 0x0) {
    printf(1,"%d %d\n", n->inum, n->type);
    n = n->next;
  }
  exit();
}
