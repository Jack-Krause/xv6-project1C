#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    startLogging();
    int pid = fork();

    if (pid < 0) {
        printf("logtest: fork failed\n");
        stopLogging();
        exit(1);
    }

    if (pid == 0) {
        int child_pid = getpid();
        // int newnice = nice(child_pid, 10);
        nice(child_pid, 10);

        // do cpu burn
        volatile int calc = 0;
        for (int i = 0; i < 100000; i++) {
            for (int j = 0; j < 10000; j++) {
                calc += i;
            }
        }

        exit(0); // child must exit explicitly
    }

    wait(0);
    stopLogging();
    exit(0);
}















