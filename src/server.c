#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "internal.h"
#include "conditions.h"
#include "parser.h"
#include "network.h"

// Обработчик сигналов
void signal_handler(int signalno);
// Входящий поток
void *input_thread(void *args);
// Исходящий поток
void *output_thread(void *args);
// Проверка, есть ли подключенные клиенты
bool is_active_clients(void);

// PID сервера
pid_t pid;
// TID потоков
pthread_t thread_in, thread_out;
// Массив подключенных клиентов
bool *clients;
// Серверный сокет
int server_sock;
// Команда, которую необходимо послать на клиент
volatile cmd_out send_command = OUT_NONE;
// Состояние, которое необходимо выставить на клиенте
volatile state_t send_state;
// Сетевые параметры клиента, на которого необходимо послать команду
struct sockaddr_in clnt_dest = {0};
socklen_t clnt_dest_len = sizeof(clnt_dest);
// База данных о сетевых параметрах всех клиентов
struct sockaddr_in clnt_base[BACKLOG] = {0};

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

    /*
    // Слушаем сокет
    struct sockaddr_in clntaddr = {0};
    socklen_t clntlen = sizeof(clntaddr);
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
    */

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
    // Пока есть подключенные клиенты,
    // продолжаем работу
    while (is_active_clients()) {
        struct sockaddr_in clntaddr = {0};
        socklen_t clntlen = sizeof(clntaddr);
        char buffer[BUFSIZ];
        memset(buffer, 0, BUFSIZ);
        // Слушаем сокет
        Recvfrom(server_sock, buffer, BUFSIZ, MSG_WAITALL, 
                (struct sockaddr *) &clntaddr, &clntlen);
        
        // Парсим информацию
        cmd_in type = IN_NONE;
        id_t clntnum;
        type = parser_in(buffer);
        switch (type) {
            // Если клиент пытается подключиться
            case IN_LAUNCH:
                sscanf(buffer, "Client #%d is launched", &clntnum);
                clnt_dest = clntaddr;
                clnt_dest_len = clntlen;
                // Если клиент с таким id уже подключен, то отклоняем подключение
                if (clients[clntnum-1]) {
                    send_command = OUT_DENY;
                }
                // Иначе отсылаем подтверждение об успешном подключении на клиент
                else {
                    clnt_base[clntnum-1] = clntaddr;
                    printf("[%d] Client #%d is connected to server\n", pid, clntnum);
                    clients[clntnum-1] = true;
                    send_command = OUT_CONN;
                }
                break;
            // Если клиент высылает свое состояние
            case IN_STATE:
                state_t cur_state;
                sscanf(buffer, "Client #%d is in state: %d", &clntnum, &cur_state);
                print_client_state(clntnum, pid, cur_state);
                break;
            // Если клиент отключается
            case IN_SHUTDOWN:
                sscanf(buffer, "Client #%d is shutdown", &clntnum);
                memset(&(clnt_base[clntnum-1]), 0, sizeof(clnt_base[clntnum-1]));
                printf("[%d] Client #%d is shutdown\n", pid, clntnum);
                clients[clntnum-1] = false;
                break;
        }
    }
    pthread_exit(EXIT_SUCCESS);
}

// Исходящий поток
void *output_thread(void *args) {
    while (true) {
        char buffer[BUFSIZ];
        memset(buffer, 0, BUFSIZ);
        // Ждем прихода команды
        while (send_command == OUT_NONE)
            Usleep(10);
        switch (send_command) {
            // В случае отклонения подключения клиента
            case OUT_DENY:
                sprintf(buffer, "Deny");
                break;
            // В случае успешного подключения клиента
            case OUT_CONN:
                sprintf(buffer, "Client is connected to server");
                break;
            // Необходимо выдать состояние клиента
            case OUT_GETSTATE:
                sprintf(buffer, "Get state");
                break;
            // Необходимо установить состояние клиента
            case OUT_SETSTATE:
                sprintf(buffer, "Set state: %d", send_state);
                break;
            // В случае отключения клиента
            case OUT_SHUTDOWN:
                sprintf(buffer, "Shutdown");
        }
        Sendto(server_sock, buffer, strlen(buffer), MSG_CONFIRM,
               (const struct sockaddr *) &clnt_dest, clnt_dest_len);
        memset(&clnt_dest, 0, sizeof(clnt_dest));
        clnt_dest_len = 0;
        send_command = OUT_NONE;
    }
    pthread_exit(EXIT_SUCCESS);
}

// Проверка, есть ли подключенные клиенты
bool is_active_clients(void) {
    for (int i = 0; i < BACKLOG; i++)
        if (clients[i])
            return true;
    return false;
}
