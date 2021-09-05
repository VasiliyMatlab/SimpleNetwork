#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "internal.h"
#include "network.h"
#include "logfile.h"

void signal_handler(int signalno);

pid_t pid;
int logfile;
bool *clients;

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
    //sleep(2);

    // Создание лог-файла
    Creat(LOG, O_RDWR);
    key_t logkey = Ftok(LOG, 0);
    logfile = Shmget(logkey, BACKLOG*sizeof(bool), IPC_CREAT | 0666);
    clients = (bool *) Shmat(logfile, NULL, 0);

    // Выключение сервера
    sleep(2000000);
    Shmdt(clients);
    Remove(LOG);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}

void signal_handler(int signalno) {
    Shmdt(clients);
    Remove(LOG);
    printf("\n[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}