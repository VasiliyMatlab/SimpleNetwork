#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "internal.h"
#include "network.h"
#include "conditions.h"
#include "parser.h"

// Обработчик сигналов
void signal_handler(int signalno);

// PID клиента
pid_t pid;
// // ID клиента
id_t id;
// Клиентский сокет
int client_sock;
// Текущее состояние клиента
state_t state = OFF;
// Сетевые параметры сервера
struct sockaddr_in servaddr = {0};
socklen_t servlen = sizeof(servaddr);

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
    
    // Создание сокета
    client_sock = Socket(AF_INET, SOCK_DGRAM, 0);
    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    servlen = sizeof(servaddr);

    // Пытаемся подключиться на сервер
    printf("[%d] Trying to connect with server\n", pid);
    char buffer[BUFSIZ];
    sprintf(buffer, "Client #%d is launched", id);
    Sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, 
           (const struct sockaddr *) &servaddr, servlen);


    while (true) {
        // Слушаем сокет
        memset(buffer, 0, BUFSIZ);
        Recvfrom(client_sock, buffer, BUFSIZ, MSG_WAITALL, 
                 (struct sockaddr *) &servaddr, &servlen);
        // Парсим информацию
        cmd_out type = OUT_NONE;
        type = parser_out(buffer);
        switch ((int) type) {
            // В случае отклонения подключения клиента
            case OUT_DENY:
                printf("[%d] Unable to connect to server\n", pid);
                printf("[%d] Termination of the process\n", pid);
                goto ending;
                break;
            // В случае успешного подключения клиента
            case OUT_CONN:
                printf("[%d] Client is connected to server\n", pid);
                print_client_state(id, pid, state);
                break;
            // Необходимо установить состояние клиента
            case OUT_SETSTATE:
                sscanf(buffer, "Set state: %d", (int *) &state);
                print_client_state(id, pid, state);
            // Необходимо выдать состояние клиента
            case OUT_GETSTATE:
                memset(buffer, 0, BUFSIZ);
                sprintf(buffer, "Client #%d is in state: %d", id, state);
                Sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, 
                    (const struct sockaddr *) &servaddr, servlen);
                break;
            // В случае отключения клиента от сервера
            case OUT_SHUTDOWN:
                memset(buffer, 0, BUFSIZ);
                sprintf(buffer, "Client #%d is shutdown", id);
                Sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, 
                    (const struct sockaddr *) &servaddr, servlen);
                goto ending;
        }
    }
    

    ending: ;
    // Выключение клиента
    Close(client_sock);
    printf("[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}

// Обработчик сигналов
void signal_handler(int signalno) {
    char buffer[BUFSIZ];
    sprintf(buffer, "Client #%d is shutdown", id);
    Sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, 
           (const struct sockaddr *) &servaddr, servlen);
    Close(client_sock);
    printf("\n[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}