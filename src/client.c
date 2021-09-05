#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "internal.h"
#include "network.h"
#include "conditions.h"

void signal_handler(int signalno);

pid_t pid;
id_t id;
int logfile;
ssize_t bytes;
char buffer[BUFSIZ];

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

    // Открытие лог-файла и запись в него клиента
    logfile = Open(LOG, O_RDWR);
    memset(buffer, 0, BUFSIZ);
    bytes = Read(logfile, buffer, BUFSIZ-1);
    sprintf(buffer, "Client %d\n", id);
    bytes = Write(logfile, buffer, strlen(buffer));

    // Запуск клиента
    pid = getpid();
    printf("[%d] Client #%d is launched\n", pid, id);
    sleep(2);
    
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
    //sleep(2);
    getchar();
    remove_client_from_log(logfile, id);
    Close(logfile);
    printf("[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}

void signal_handler(int signalno) {
        Close(logfile);
        printf("\n[%d] Client #%d is shutdown\n", pid, id);
        exit(EXIT_SUCCESS);
}