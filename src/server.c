#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    printf("[%d] Server is launched\n", pid);
    sleep(2);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}