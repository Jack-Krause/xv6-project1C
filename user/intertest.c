#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CPU_WORK_ITERS   80000000
#define BURST_WORK_ITERS 2000000
#define NUM_BURSTS       20


static void cpu_bound(void) {
    volatile int x = 0;
    for (int i = 0; i < CPU_WORK_ITERS; i++) {
        x += i;
    }
}

static void interactive_like(void) {
    volatile int x = 0;
    
    for (int b = 0; b < NUM_BURSTS; b++) {
        for (int i = 0; i < BURST_WORK_ITERS; i++) {
            x += i;
        }

        // sleep(1);
        pause(1);
    }
}


int main(int argc, char *argv[]) {
    int pid_cpu0, pid_cpu1, pid_int0, pid_int1;

    printf("intertest: 2 cpu-bound, 2 interactive children\n");

    startLogging();

    pid_cpu0 = fork();
    if (pid_cpu0 == 0) {
        printf("[intertest cpu0 pid=%d], starting\n", getpid());
        cpu_bound();
        printf("[intertest cpu0 pid=%d] done\n", getpid());
        exit(0);
    }


    pid_cpu1 = fork();
    if (pid_cpu1 == 0) {
        printf("[intertest cpu1 pid=%d], starting\n", getpid());
        cpu_bound();
        printf("[intertest cpu1 pid=%d] done\n", getpid());
        exit(0);
    }


    pid_int0 = fork();
    if (pid_int0 == 0) {
        printf("[intertest int0 pid=%d], starting\n", getpid());
        interactive_like();
        printf("[intertest int0 pid=%d] done\n", getpid());
        exit(0);
    }


    pid_int1 = fork();
    if (pid_int1 == 0) {
        printf("[intertest int1 pid=%d], starting\n", getpid());
        interactive_like();
        printf("[intertest int1 pid=%d] done\n", getpid());
        exit(0);
    }


    wait(0);
    wait(0);
    wait(0);
    wait(0);

    printf("intertest: children finished\n");
    stopLogging();
    exit(0);
}



