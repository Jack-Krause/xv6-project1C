# JACK KRAUSE - COMS 3520 project 1c

### Summary
For this project, we implement the following new schedulers for the xv6 OS:

1. round-robin scheduler with strict priorities (RRSP), and
2. multi-level feedback queue (MLFQ)

### RRSP rules
for any processes A and B:

1. if **priority(A) > priority(B)**, A runs (B doesn't)
2. if **priority(A) == priority(B)**, A and B run in round-robin fashion with a 1-tick quanta
3. The priority of a process X is related to the nice value of the process based on this formula: **priority(X) = 20 - nice(X)**. Note: lower nice means higher priority.


### MLFQ rules
for any processes A and B:
1. if **priority(A) > priority(B)**, A runs (B doesn't)
2. if **priority(A) == priority(B)**, A and B run in round-robin fashion using the quantum length of the given queue (see table below)
3. When a process enters the system, its starting queue is determined by its nice value (see table below).
4. Once a process uses up its time allotment at a given level (regardless of how many times its given up the CPU), its priority is reduced (moves down one queue).
5. After every 60 ticks, all processes in the system are moved back to their starting queues (*priority boost*).
6. Whenever the nice value of a process is updated (*nice() system call*), its queue level is also changed to be the starting queue level for that nice value, according to the table below. Note: this may result in the process moving to another queue.

## MLFQ Queue Levels Based on Nice Values

| Queue Level | Nice Value Range            | Time Quantum (ticks) |
|-------------|------------------------------|------------------------|
| **Q2 (highest)** | Nice ≤ -10                     | 1                      |
| **Q1**           | -10 < Nice ≤ 10                | 10                     |
| **Q0 (lowest)**  | Nice > 10                      | 15                     |



# JACK KRAUSE - COMS 3520 project 1b

### Summary
This project extends the xv6 kernel by adding 3 new system calls and modifying the scheduler to support kernel logging and process nice values.

## New System Calls
#22: `startLogging()` - enables context switch logging in the kernel scheduler

#23: `stopLogging()` - disables scheduler logging

#24: `nice(pid, inc)` - Adjusts the nice value of the process with pid by inc (bounded from -20 to +19)

### Scheduler Logging Format
running `pid` at `ticks`

Where `ticks` is the global tick counter.

## Running `$ logtest`
```
xv6 kernel is booting

init: starting sh
$ logtest
Logging Started
running 4 at 33
nice set to 10 for 4
running 4 at 34
running 4 at 35
running 4 at 36
running 4 at 37
running 4 at 38
running 4 at 39
running 4 at 40
running 4 at 41
running 4 at 42
running 4 at 43
running 4 at 44
running 4 at 45
running 4 at 46
running 4 at 47
running 4 at 48
running 4 at 49
running 4 at 50
running 4 at 51
running 4 at 52
running 4 at 53
running 4 at 54
running 4 at 55
running 4 at 56
running 4 at 57
running 4 at 58
running 4 at 59
running 4 at 60
running 4 at 61
running 4 at 62
running 4 at 63
running 4 at 64
running 4 at 65
running 4 at 66
running 4 at 67
running 4 at 68
running 4 at 69
running 4 at 70
running 4 at 71
running 4 at 72
running 3 at 72
Logging Stopped
$ 
```


### Explanation
- `Logging Started` → `sys_startLogging()` enabled scheduler output.  
- Each `running 4 at <t>` shows the kernel about to context-switch to PID 4.  
- `nice set to 10 for 4` comes from `sys_nice()`.  
- Continuous lines show the child running while burning CPU.  
- `running 3 at 72` marks the shell (PID 3) regaining control after the child exits.  


## Test — Bounded Nice Values [–20, 19]
Note: logging stopped to avoid breaking up of outputs via preemption

xv6 kernel output:
```
xv6 kernel is booting

init: starting sh
$ logtest
Logging Started
running 4 at 146
Logging Stopped
nice after +1000, nice=19, exp=19
nice after - 10k, nice=-20, exp=(-20)
nice after +5, nice=-15, exp=-15
Logging Started
running 3 at 146
Logging Stopped
$ 
```

## Implementation Details
### `kernel/proc.c`
- Added `static int logging_enabled = 0;`
- Implemented `sys_startLogging()`, `sys_stopLogging()`, `sys_nice()`
- Added logging print inside `scheduler()` just before `swtch()`
- Initialized `p->nice = 0` for new processes

### `kernel/syscall.h`
- Added `SYS_startLogging 22`, `SYS_stopLogging 23`, `SYS_nice 24`

### `kernel/syscall.c`
- Added corresponding entries to the syscall table

### `user/user.h`, `user/usys.pl`
- Added user-space prototypes and entries for `startLogging()`, `stopLogging()`, and `nice(pid, inc)`

### `user/logtest.c`
- Starts logging → `fork()` → child sets nice = 10 and burns CPU → parent `wait()` → stop logging → exit

---

## Build and Run
```sh
make clean && make qemu
# In xv6:
$ logtest
```



**XV6 Info**

xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)).  See also https://pdos.csail.mit.edu/6.1810/, which provides
pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Abhinavpatel00, Takahiro Aoyagi, Marcelo Arroyo, Hirbod Behnam, Silas
Boyd-Wickizer, Anton Burtsev, carlclone, Ian Chen, clivezeng, Dan
Cross, Cody Cutler, Mike CAT, Tej Chajed, Asami Doi,Wenyang Duan,
echtwerner, eyalz800, Nelson Elhage, Saar Ettinger, Alice Ferrazzi,
Nathaniel Filardo, flespark, Peter Froehlich, Yakir Goaron, Shivam
Handa, Matt Harvey, Bryan Henry, jaichenhengjie, Jim Huang, Matúš
Jókay, John Jolly, Alexander Kapshuk, Anders Kaseorg, kehao95,
Wolfgang Keller, Jungwoo Kim, Jonathan Kimmitt, Eddie Kohler, Vadim
Kolontsov, Austin Liew, l0stman, Pavan Maddamsetti, Imbar Marinescu,
Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi Merimovich,
mes900903, Mark Morrissey, mtasm, Joel Nider, Hayato Ohhashi,
OptimisticSide, papparapa, phosphagos, Harry Porter, Greg Price, Zheng
qhuo, Quancheng, RayAndrew, Jude Rich, segfault, Ayan Shafqat, Eldar
Sehayek, Yongming Shen, Fumiya Shigemitsu, snoire, Taojie, Cam Tenny,
tyfkda, Warren Toomey, Stephen Tu, Alissa Tung, Rafael Ubal, unicornx,
Amane Uehara, Pablo Ventura, Luc Videau, Xi Wang, WaheedHafez, Keiichi
Watanabe, Lucas Wolf, Nicolas Wolovick, wxdao, Grant Wu, x653, Andy
Zhang, Jindong Zhang, Icenowy Zheng, ZhUyU1997, and Zou Chang Wei.

ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu).  The main purpose of xv6 is as a teaching
operating system for MIT's 6.1810, so we are more interested in
simplifications and clarifications than new features.

BUILDING AND RUNNING XV6

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu.  Once they are installed, and in your shell
search path, you can run "make qemu".
