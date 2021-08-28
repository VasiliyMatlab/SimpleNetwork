#include "network.h"

int main() {
    pid_t pid = getpid();
    printf("[%d] Client is launched\n", pid);
    sleep(2);
    printf("[%d] Client is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}