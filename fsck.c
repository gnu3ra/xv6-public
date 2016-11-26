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


struct dirlink {
  uint dirnum;
  uint linkto; 
};

struct filelink {
  uint filenum;
  uint numlinks; 
};



struct filelink * files;
struct dirlink * dirs;

uint filecounter;
uint dircounter;
uint devcounter;
uint othercounter;

int checklinks(struct unode * first, struct unode * second) {
  dirs = malloc(sizeof(struct dirlink)*dircounter);
  files = malloc(sizeof(struct filelink) * filecounter);

  int fcounter;
  int dcounter;
  fcounter = 0;
  dcounter = 0;
  while(first->next != 0x0) {
    struct filelink f;
    f.filenum = first->inum;
    f.numlinks = first->nlinks;
    files[dcounter] = f;
    dcounter++;
    first = first->next;
    
  }

  fcounter = 0;
  dcounter = 0;
  while(second->next != 0x0) {
    second = second->next;
    fcounter++;
  }
  return 0;
}

int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();
  struct unode * dw = first;
  struct unode * iw = second;
  
  printf(1,"compare ret value %d\n", ucompare(first,second));

  filecounter = 0;
  dircounter = 0;
  devcounter = 0;
  othercounter = 0;
  struct unode * dw_enum;
  dw_enum = dw;
  while(dw_enum->next != 0x0) {
    if(dw_enum->type == T_FILE)
      filecounter++;
    if(dw_enum->type == T_DIR)
      dircounter++;
    if(dw_enum->type == T_DEV)
      devcounter++;
    else 
      othercounter++;
    dw_enum = dw_enum->next;
  }

  struct unode * dw_check;
  struct unode * iw_check;
  iw_check = iw;
  dw_check = dw;
  checklinks(dw_check, iw_check);
  
  exit();
}
