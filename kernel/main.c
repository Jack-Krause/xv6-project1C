#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  // if (SCHEDULER == SCHED_RR) {
  //   scheduler();
  // } else if (SCHEDULER == SCHED_RRSP) {
  //   scheduler_rrsp();
  // } else if (SCHEDULER == SCHED_MLFQ) {
  //   scheduler_mlfq();
  // }

  // implemented the above logic as a preprocessor directive
  //   so the scheduler is selected at compile time.
  // this means that only the chosen scheduler needs to be compiled and linked,
  //   and unused schedulers can be ommited or incomplete
  #if SCHEDULER == SCHED_RR
    scheduler();
  #elif SCHEDULER == SCHED_RRSP
    scheduler_rrsp();
  #elif SCHEDULER == SCHED_MLFQ
    scheduler_mlfq();
  #else
  #error "scheduler not selected"
  #endif



}
