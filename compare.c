#include "compare.h"
#include "types.h"
#include "iwalk.h"
#include "stat.h"
#include "fcntl.h"
#include "param.h"
#include "user.h"
#include "fs.h"
#include "syscall.h"
#include "memlayout.h"

int ucompare(struct unode* first, struct unode* second) {
  int faulty = 0;
  struct unode  * secondfirst;
  secondfirst = second;
  if(second->size != first->size) {
    printf(1, "second: %d, first: %d\n", second->size, first->size);
    return -1;
  }
  while(first != 0x0) {
     //printf(1,"firstcounter %d\n",firstcounter);
     while(second != 0x0) {
       // printf(1, "secondcounter %d\n", secondcounter);
       if(second->inum == first->inum) {
         if(second->type != first->type) {
           faulty = 1;
         }
         else {
//           printf(1,"GHaaaaa [%d] [%d]\n",first->type,  second->type);
           break;
         }
       }
       second = second->next;
     }
     second = secondfirst; 
     first = first->next;
   }
   if(faulty)
     return -2;

   return 0;
}


