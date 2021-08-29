#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "network.h"
#include "conditions.h"

int main(int argc, char *argv[]) {
    // Обработка аргументов командной строки
    if (argc == 1) {
        fprintf(stderr, "Arguments error: not ehough arguments\n");
        exit(EXIT_FAILURE);
    }
    if  (argc > 2) {
        fprintf(stderr, "Arguments error: too many arguments\n");
        exit(EXIT_FAILURE);
    }
    id_t id = atoi(argv[1]);
    if ((id == 0) || (id > BACKLOG)) {
        fprintf(stderr, "ID error: invalid number\n"
            "ID must be in [1..%d]\n", BACKLOG);
        exit(EXIT_FAILURE);
    }

    // Запуск клиента
    pid_t pid = getpid();
    printf("[%d] Client #%d is launched\n", pid, id);
    sleep(2);
    
    // Инициализация и изменение состояний клиента
    state_t state = OFF;
    print_client_state(id, pid, state);
    sleep(2);
    state = YELLOW | RED;
    print_client_state(id, pid, state);
    sleep(2);
    state = ALL;
    print_client_state(id, pid, state);

    // Выключение клиента
    sleep(2);
    printf("[%d] Client #%d is shutdown\n", pid, id);
    exit(EXIT_SUCCESS);
}