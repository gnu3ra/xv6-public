#include "types.h"
#include "dwalk.h"
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
  int firstcounter = 0;
  int secondcounter = 0;
  int secondmax = 0;
  struct unode  * secondfirst;
  secondfirst = second;
  //struct unode * firstfirst;
  //firstfirst = first;
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
           secondcounter++;
           break;
         }
       }
       second = second->next;
       secondcounter++;
     }
     if(secondcounter > secondmax)
       secondmax = secondcounter;
     secondcounter = 0;
     second = secondfirst; 
     first = first->next;
     firstcounter++;
   }
   if(secondmax != firstcounter) {
     printf(1, "Secondmax: %d Firstcounter: %d\n",secondmax, firstcounter);
     return -1;
   }
   if(faulty)
     return -2;

   return 0;
}


int main(void) {
  struct unode * first = dwalk("/");
  struct unode * second = iwalk();

  printf(1,"compare ret value %d\n", ucompare(first,second));
  exit();
}
