#include "network.h"

int main() {
    pid_t pid = getpid();
    printf("[%d] Server is launched\n", pid);
    sleep(2);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}