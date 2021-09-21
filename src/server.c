#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "internal.h"
#include "parser.h"
#include "network.h"

// Обработчик сигналов
void signal_handler(int signalno);
// Входящий поток
void *input_thread(void *args);
// Исходящий поток
void *output_thread(void *args);

pid_t pid;
pthread_t thread_in, thread_out;
bool *clients;
int server_sock;
struct sockaddr_in clntaddr = {0};
socklen_t clntlen = sizeof(clntaddr);

int main() {
    // Задаем обработчик сигналов
    void (*status)(int);
    status = signal(SIGINT, signal_handler);
    status = signal(SIGTERM, signal_handler);
    if (status == SIG_ERR) {
        perror("signal handler failed");
        exit(EXIT_FAILURE);
    }

    // Создаем потоки
    Pthread_create(&thread_in,  NULL, input_thread,  NULL);
    Pthread_create(&thread_out, NULL, output_thread, NULL);

    // Запуск сервера
    pid = getpid();
    printf("[%d] Server is launched\n", pid);
    //sleep(2);

    // Лог клиентов
    clients = (bool *) malloc(BACKLOG*sizeof(bool));
    for (int i = 0; i < BACKLOG; i++)
        clients[i] = false;

    // Создание сокета
    server_sock = Socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {0};
    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Связывание сокета с адресом сервера
    Bind(server_sock, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    // Слушаем сокет
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);
    Recvfrom(server_sock, buffer, BUFSIZ, MSG_WAITALL, 
             (struct sockaddr *) &clntaddr, &clntlen);

    // Парсим информацию
    id_t clntnum = -1;
    sscanf(buffer, "Client #%d is launched", &clntnum);
    memset(buffer, 0, BUFSIZ);
    // Если клиент с таким id уже подключен, то отклоняем подключение
    if (clients[clntnum-1]) {
        sprintf(buffer, "Deny");
        Sendto(server_sock, buffer, strlen(buffer), MSG_CONFIRM,
               (const struct sockaddr *) &clntaddr, clntlen);
    }
    // Иначе отсылаем подтверждение об успешном подключении на клиент
    else {
        printf("[%d] Client #%d is connected to server\n", pid, clntnum);
        clients[clntnum-1] = true;
        sprintf(buffer, "Client #%d is connected to server", clntnum);
        Sendto(server_sock, buffer, strlen(buffer), MSG_CONFIRM, 
            (const struct sockaddr *) &clntaddr, clntlen);
    }

    // Выключение сервера
    sleep(5);
    Pthread_join(thread_in, NULL);
    Pthread_join(thread_out, NULL);
    free(clients);
    Close(server_sock);
    printf("[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}

// Обработчик сигналов
void signal_handler(int signalno) {
    free(clients);
    Close(server_sock);
    printf("\n[%d] Server is shutdown\n", pid);
    exit(EXIT_SUCCESS);
}

// Входящий поток
void *input_thread(void *args) {
    printf("Hello from input_thread!\n");
    pthread_exit(EXIT_SUCCESS);
}

// Исходящий поток
void *output_thread(void *args) {
    printf("Hello from output_thread!\n");
    pthread_exit(EXIT_SUCCESS);
}
