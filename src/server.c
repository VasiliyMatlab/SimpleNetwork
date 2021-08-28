#include "network.h"
#include "conditions.h"

int main() {
    pid_t pid = getpid();
    printf("[%d] Server is launched\n", pid);
    sleep(2);

    int logfile = open(LOG, O_RDWR | O_CREAT, 0644);

    sleep(2);
    close(logfile);
    remove(LOG);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}