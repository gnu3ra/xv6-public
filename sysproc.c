#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  proc->trapcount++;
  return fork();
}

int
sys_exit(void)
{
  proc->trapcount++;
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  proc->trapcount++;
  return wait();
}

int
sys_kill(void)
{
  proc->trapcount++;
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  proc->trapcount++;
  return proc->pid;
}

int
sys_sbrk(void)
{
  proc->trapcount++;
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  proc->trapcount++;
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  proc->trapcount++;
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
 

int
sys_gticks(void)
{
  proc->trapcount++;
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  
  return xticks - proc->startticks;
}


int
sys_tcount(void)
{
  // proc->trapcount++; //THIS MAY BE WRONG. Not sure if it should do this.
  return proc->trapcount;
}

int
sys_icount(void) {
  proc->trapcount++;
  return getnuminodes();
}

