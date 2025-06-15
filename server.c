#include "server.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void start_server() {
    const char *names[] = {"Anna", "Paul", "Rick", "Nina"};
    int pipes[4][2][2]; // [client][read/write][0:read,1:write]
    pid_t pids[4];

    for (int i = 0; i < 4; ++i) {
        pipe(pipes[i][0]); // parent -> child
        pipe(pipes[i][1]); // child -> parent

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            close(pipes[i][0][1]); // close write end of parent->child
            close(pipes[i][1][0]); // close read end of child->parent
            client_process(names[i], pipes[i][0][0], pipes[i][1][1]);
            exit(0);
        } else {
            // Parent process
            pids[i] = pid;
            close(pipes[i][0][0]); // close read end of parent->child
            close(pipes[i][1][1]); // close write end of child->parent
        }
    }

    sleep(1);
    for (int i = 0; i < 4; ++i)
        write(pipes[i][0][1], "Hello clients! Who are you?\n", 29);
    sleep(1);
    for (int i = 0; i < 4; ++i) {
        char buf[128];
        read(pipes[i][1][0], buf, sizeof(buf));
        printf("Server received: %s", buf);
    }

    sleep(1);
    for (int i = 0; i < 4; ++i)
        write(pipes[i][0][1], "Hello clients! Your PID?\n", 26);
    sleep(1);
    for (int i = 0; i < 4; ++i) {
        char buf[128];
        read(pipes[i][1][0], buf, sizeof(buf));
        printf("Server received: %s", buf);
    }

    sleep(1);
    for (int i = 0; i < 4; ++i)
        kill(pids[i], SIGUSR2);
    sleep(1);
    for (int i = 0; i < 4; ++i)
        kill(pids[i], SIGTERM);
    sleep(1);
    for (int i = 0; i < 4; ++i)
        kill(pids[i], SIGINT);

    for (int i = 0; i < 4; ++i)
        waitpid(pids[i], NULL, 0);
}
