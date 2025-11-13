#define NPROC        64  // maximum number of processes
#define NCPU          8  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGBLOCKS    (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       2000  // size of file system in blocks
#define MAXPATH      128   // maximum file path name
#define USERSTACK    1     // user stack pages
#define SCHED_RR     0     // default round-robin scheduler
#define SCHED_RRSP   1     // 1c(3.1) implemented round-robin scheduler with strict priorities (RRSP)
#define SCHED_MLFQ   2     // 1c(3.1) implemented multi-level feedback queue (MLFQ)
#define SCHEDULER    SCHED_RR  // 1c(3.1) change this line to select a different scheduler

