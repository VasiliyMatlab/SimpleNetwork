#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    printf("[%d] Client is launched\n", pid);
    sleep(2);
    printf("[%d] Client is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}