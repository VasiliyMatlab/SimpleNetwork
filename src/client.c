#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include "internal.h"
#include "network.h"
#include "logfile.h"
#include "conditions.h"

void signal_handler(int signalno);

pid_t pid;
id_t id;
int logfile;
bool *clients;

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
    key_t logkey = Ftok(LOG, 0);
    logfile = Shmget(logkey, BACKLOG*sizeof(bool), 0666);
    clients = (bool *) Shmat(logfile, NULL, 0);
    if (clients[id-1] == true) {
        fprintf(stderr, "Client error: client with id = %d is already in use\n", id);
        exit(EXIT_FAILURE);
    }
    clients[id-1] = true;

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
    sleep(2);
    clients[id-1] = false;
    Shmdt(clients);
    printf("[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}

void signal_handler(int signalno) {
    clients[id-1] = false;
    Shmdt(clients);
    printf("\n[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}