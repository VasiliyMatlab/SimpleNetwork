#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "internal.h"
#include "network.h"
#include "conditions.h"

// Обработчик сигналов
void signal_handler(int signalno);

pid_t pid;
id_t id;
int client_sock;
ssize_t bytes;

int main(int argc, char *argv[]) {
    // Задаем обработчик сигналов
    void (*status)(int);
    status = signal(SIGINT, signal_handler);
    status = signal(SIGTERM, signal_handler);
    if (status == SIG_ERR) {
        perror("signal handler failed");
        exit(EXIT_FAILURE);
    }
    
    // Обработка аргументов командной строки
    if (argc == 1) {
        fprintf(stderr, "Arguments error: not ehough arguments\n");
        exit(EXIT_FAILURE);
    }
    if  (argc > 2) {
        fprintf(stderr, "Arguments error: too many arguments\n");
        exit(EXIT_FAILURE);
    }
    id = atoi(argv[1]);
    if ((id == 0) || (id > BACKLOG)) {
        fprintf(stderr, "ID error: invalid number\n"
            "ID must be in [1..%d]\n", BACKLOG);
        exit(EXIT_FAILURE);
    }

    // Запуск клиента
    pid = getpid();
    printf("[%d] Client #%d is launched\n", pid, id);
    sleep(2);
    
    // Создание сокета
    client_sock = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {0};
    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    socklen_t servlen = sizeof(servaddr);

    // Посылаем информацию о клиенте на сервер
    char buffer[BUFSIZ];
    sprintf(buffer, "Client #%d is launched", id);
    Sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, 
           (const struct sockaddr *) &servaddr, servlen);

    // Принимаем ответ сервера
    memset(buffer, 0, BUFSIZ);
    bytes = Recvfrom(client_sock, buffer, BUFSIZ, MSG_WAITALL, 
                     (struct sockaddr *) &servaddr, &servlen);
    id_t tmp = -1;
    if (!strcmp(buffer, "Deny")) {
        fprintf(stderr, "Connection error: client with id = %d "
            "is already exist\n", id);
        Close(client_sock);
        exit(EXIT_FAILURE);
    }
    sscanf(buffer, "Client #%d is connected to server", &tmp);
    if (tmp == id)
        printf("[%d] Client #%d is connected to server\n", pid, id);
    else {
        fprintf(stderr, "Connection error: wrong received id from server\n");
        fprintf(stderr, "Connection is terminated\n");
        Close(client_sock);
        exit(EXIT_FAILURE);
    }

    // Инициализация и изменение состояний клиента
    state_t state = OFF;
    print_client_state(id, pid, state);
    //sleep(2);
    state = YELLOW | RED;
    print_client_state(id, pid, state);
    //sleep(2);
    state = ALL;
    print_client_state(id, pid, state);

    // Выключение клиента
    sleep(2);
    Close(client_sock);
    printf("[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}

// Обработчик сигналов
void signal_handler(int signalno) {
    Close(client_sock);
    printf("\n[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}