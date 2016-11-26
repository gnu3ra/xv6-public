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



uint filecounter;
uint dircounter;
uint devcounter;
uint othercounter;

int checklinks(struct unode * first, struct unode * second) {
  return 0;
}


void enumerate(struct unode * first) {
  

}

int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();
  //struct unode * dw = first;
  struct unode * iw = second;
  
  printf(1,"compare ret value %d\n", ucompare(first,second));

  filecounter = 0;
  dircounter = 0;
  devcounter = 0;
  othercounter = 0;
  struct unode * iw_enum;
  iw_enum = iw;
  while(iw_enum->next != 0x0) {
    if(iw_enum->type == T_FILE)
      filecounter++;
    if(iw_enum->type == T_DIR)
      dircounter++;
    if(iw_enum->type == T_DEV)
      devcounter++;
    else 
      othercounter++;
    iw_enum = iw_enum->next;
  }
  
  exit();
}
