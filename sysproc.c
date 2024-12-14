#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern int count_free_pages(void);
extern int count_active_processes(void);

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// return current time and date
int
sys_date(void)
{
  struct rtcdate* r;

  if(argptr(0, (void*)&r, sizeof(*r)) < 0)
    return -1;

  cmostime(r);
  return 0;
}

// return number of processes
int
sys_numproc(void)
{
  return count_active_processes();
}

// return number of open files
int
sys_numopenfiles(void)
{
  struct proc *p = myproc(); // current process
  int count = 0;

  for(int i = 0; i < NOFILE; i++){
      if (p->ofile[i]) {
          count++;
      }
  }
  return count;
}

// return number of free memory pages
int
sys_freememp(void)
{
  return count_free_pages();
}

// return size of process memory (bytes)
int
sys_spacesize(void)
{
  struct proc *p = myproc();  // current process
  return p->sz;
}
