#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CPU_WORK_ITERS 80000000

static void cpu_bound(void) {
    volatile int x = 0;
    for (int i = 0; i < CPU_WORK_ITERS; i++) {
        x += i;
    }
}

int
main(int argc, char *argv[])
{
    printf("fairtest: starting 4 cpu bound children (equal nice)\n");
    startLogging();

    for (int i = 0; i < 4; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("fairtest: fork failed\n");
            exit(1);
        }
        if (pid == 0) {
            printf("[child%d pid=%d] starting\n", i, getpid());
            cpu_bound();
            printf("[child%d pid=%d] done\n", i, getpid());
            exit(0);
        }
    }

    for (int i = 0; i < 4; i++)
        wait(0);

    printf("fairtest: children finished\n");
    stopLogging();
    exit(0);
}
