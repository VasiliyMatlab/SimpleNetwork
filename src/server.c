#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "internal.h"
#include "network.h"
#include "conditions.h"

int main() {
    // Запуск сервера
    pid_t pid = getpid();
    printf("[%d] Server is launched\n", pid);
    sleep(2);

    // Создание лог-файла
    //int logfile = open(LOG, O_RDWR | O_CREAT, 0644);
    int logfile = Creat(LOG, 0644);

    // Выключение сервера
    sleep(2);
    Close(logfile);
    Remove(LOG);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}