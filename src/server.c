#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int server;
ssize_t bytes;

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

    // Создание сокета
    server = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {0};
    struct sockaddr_in clntaddr = {0};
    socklen_t clntlen = sizeof(clntaddr);
    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Связывание сокета с адресом сервера
    Bind(server, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    // Слушаем сокет
    char buffer[BUFSIZ];
    bytes = Recvfrom(server, buffer, BUFSIZ, MSG_WAITALL, 
                     (struct sockaddr *) &clntaddr, &clntlen);
    buffer[bytes] = '\0';

    // Парсим информацию
    id_t clntnum = -1;
    sscanf(buffer, "Client #%d is on\n", &clntnum);
    printf("[%d] Client #%d is connected to server\n", pid, clntnum);

    // Отсылаем подтверждение об успешном подключении на клиент
    memset(buffer, 0, BUFSIZ);
    sprintf(buffer, "Client #%d is connected to server\n", clntnum);
    Sendto(server, buffer, strlen(buffer), MSG_CONFIRM, 
           (const struct sockaddr *) &clntaddr, clntlen);

    // Выключение сервера
    sleep(2000000);
    Shmdt(clients);
    Close(server);
    Remove(LOG);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}

// Обработчик сигналов
void signal_handler(int signalno) {
    Shmdt(clients);
    Close(server);
    Remove(LOG);
    printf("\n[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}