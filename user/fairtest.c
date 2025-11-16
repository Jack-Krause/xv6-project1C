#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WORK_ITERS 100000000

static void cpu_bound() {
    volatile int x = 0;
    for (int i = 0; i < WORK_ITERS; i++) {
        x += i;
    }
}


int main(int argc, char *argv[]) {
    int pid0, pid1, pid2, pid3;

    printf("fairtest: starting 4 cpu bound children\n");

    startLogging();

    // child 0
    pid0 = fork();
    if (pid0 == 0) {
        printf("[child0 pid=%d] starting\n", getpid());
        cpu_bound();
        printf("[child0 pid=%d] done \n", getpid());
        exit(0);
    }

    // child 1
    pid1 = fork();
    if (pid1 == 0) {
        printf("[child1 pid=%d] starting\n", getpid());
        cpu_bound();
        printf("[child1 pid=%d] done\n", getpid());
        exit(0);
    }

    // child 2
    pid2 = fork();
    if (pid2 == 0) {
        printf("[child2 pid=%d] starting\n", getpid());
        cpu_bound();
        printf("[child2 pid=%d] done\n", getpid());
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
        printf("[child3 pid=%d] starting\n", getpid());
        cpu_bound();
        printf("[child3 pid=%d] done\n", getpid());
        exit(0);
    }

    nice(pid0, -5);
    nice(pid1, -5);
    nice(pid2, +5);
    nice(pid3, +5);

    printf("fairtest: nice values set:\n");
    printf("   pid %d : nice -5\n", pid0);
    printf("   pid %d : nice -5\n", pid1);
    printf("   pid %d : nice +5\n", pid2);
    printf("   pid %d : nice +5\n", pid3);

    wait(0);
    wait(0);
    wait(0);
    wait(0);

    printf("fairtest: children finished\n");
    stopLogging();
    exit(0);
        
    





}