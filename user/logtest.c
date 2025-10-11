#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // TODO...

    startLogging();
    int pid = fork();
    if (pid < 0) {
        printf("logtest: fork failed\n");
        stopLogging();
        exit(1);
    }

    if (pid == 0) {
       int child_pid = getpid();

       int newnice = nice(child_pid, 10);
       // do cpu burn
    }

    stopLogging();
    exit(0);
}















