#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "internal.h"
#include "network.h"
#include "conditions.h"

void signal_handler(int signalno);

pid_t pid;
int logfile;

int main() {
    signal(SIGINT, signal_handler);

    // Запуск сервера
    pid = getpid();
    printf("[%d] Server is launched\n", pid);
    sleep(2);

    // Создание лог-файла
    logfile = Creat(LOG, 0644);

    // Выключение сервера
    sleep(20);
    Close(logfile);
    Remove(LOG);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}

void signal_handler(int signalno) {
    if (signalno == SIGINT) {
        Close(logfile);
        Remove(LOG);
        printf("\n[%d] Server is shutdown\n", pid);
        exit(EXIT_SUCCESS);
    }
}