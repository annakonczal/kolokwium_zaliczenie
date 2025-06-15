#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static const char *client_name;

void handle_signal(int sig) {
    if (sig == SIGTERM) {
        printf("%s: Terminating...\n", client_name);
    } else if (sig == SIGUSR2) {
        printf("%s: Acknowledged\n", client_name);
    } else if (sig == SIGINT) {
        printf("%s: Shutting down\n", client_name);
        exit(0);
    }
}

void client_process(const char *name, int read_fd, int write_fd) {
    client_name = name;
    printf("%s: Client initialized\n", name);

    signal(SIGTERM, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal);

    char buffer[256];

    while (1) {
        ssize_t n = read(read_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';

        if (strcmp(buffer, "Hello clients! Who are you?\n") == 0) {
            snprintf(buffer, sizeof(buffer), "I am %s\n", name);
            write(write_fd, buffer, strlen(buffer));
        } else if (strcmp(buffer, "Hello clients! Your PID?\n") == 0) {
            snprintf(buffer, sizeof(buffer), "%s: PID is %d\n", name, getpid());
            write(write_fd, buffer, strlen(buffer));
        }
    }
}
