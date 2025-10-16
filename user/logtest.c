// logtest.c
// COMS3520 project1b - Jack Krause
// test program for startLogging(), stopLogging(), and nice() system calls.
// forks a child, adjusts its nice value, spends time on CPU, and logs context switches
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    startLogging(); // enable kernel scheduler logging
    int pid = fork(); // fork child process

    if (pid < 0) {
        printf("logtest: fork failed\n");
        stopLogging();
        exit(1);
    }

    if (pid == 0) {
        int child_pid = getpid();
        // int newnice = nice(child_pid, 10);
        nice(child_pid, 10); // raise nice value of child by 10

        // do cpu burn
        volatile int calc = 0;
        for (int i = 0; i < 100000; i++) {
            for (int j = 0; j < 10000; j++) {
                calc += i;
            }
        }

        exit(0); // child must exit explicitly
    }

    wait(0); // parent process waits for child
    stopLogging(); // disable logging
    exit(0);
}















