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
char buffer[BUFSIZ];

int main() {
    // Задаем обработчик сигналов
    void (*status)(int);
    status = signal(SIGINT, signal_handler);
    status = signal(SIGTERM, signal_handler);
    if (status == SIG_ERR) {
        perror("signal handler failed");
        exit(EXIT_FAILURE);
    }

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
        Close(logfile);
        Remove(LOG);
        printf("\n[%d] Server is shutdown\n", pid);
        exit(EXIT_SUCCESS);
}