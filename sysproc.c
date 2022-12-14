#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stest.h"

int
sys_fork(void)
{
  int tickets;

  if(argint(0, &tickets) < 0)
    return -1;

  if(tickets < MINIMUM_TICKETS)
    tickets = MINIMUM_TICKETS;
  else if(tickets > MAXIMUM_TICKETS)
    tickets = MAXIMUM_TICKETS;

  return fork(tickets);
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

StestProcs procs_num_times_scheduled;
int last_position = 0;
int num_calls = 0;

int
sys_stest(void)
{
  num_calls++;
  int proc_pid;

  if(argint(0, &proc_pid) < 0)
    return -1;
  
  if(num_calls <= TEST_PROCESSES){
    cprintf("proc_pid: %d\n", proc_pid);
    stest_save_pid(proc_pid);
  }
  else{
    stest_print();
    last_position = 0;
    num_calls = 0;
  }
  
  return 0;
}

void
stest_save_pid(int pid)
{
  procs_num_times_scheduled.procs_pids_times_scheduled[last_position][0] = pid;
  procs_num_times_scheduled.procs_pids_times_scheduled[last_position++][1] = 0;
}

StestProcs*
stest_get_procs_num_times_scheduled()
{
  return &procs_num_times_scheduled;
}

void
stest_print()
{
  float total_schedules = 0.0;
  for(int i=0; i<TEST_PROCESSES; i++)
    total_schedules += procs_num_times_scheduled.procs_pids_times_scheduled[i][1];

  for(int i=0; i<TEST_PROCESSES; i++){
    int pid = procs_num_times_scheduled.procs_pids_times_scheduled[i][0];
    int times_scheduled = procs_num_times_scheduled.procs_pids_times_scheduled[i][1];
    float percentage = (float) times_scheduled / total_schedules * 100.0;
    int percentage_upper_part = (int) percentage;
    int percentage_lower_part = (percentage - (int) percentage) * 100;

    cprintf("PID: %d, TIMES SCHEDULED: %d, PERCENTAGE: %d.%d%% \n", pid, times_scheduled, percentage_upper_part, percentage_lower_part);
  }
}