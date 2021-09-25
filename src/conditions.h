#ifndef CONDITIONS_H
#define CONDITIONS_H

// Заголовочные файлы из стандартной библиотеки C
#include <stdbool.h>

// Состояния клиентов
typedef enum {
    OFF      = 0b0000,
    RED      = 0b0001,
    YELLOW   = 0b0010,
    GREEN    = 0b0100,
    ALL      = 0b0111
} state_t;

// Вывод на экран состояния клиента
void print_client_state(id_t id, pid_t pid, state_t state);
// Перевод строки в состояние
state_t str2state(char *str, pid_t pid);

#endif