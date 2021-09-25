#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conditions.h"
#include "internal.h"
#include "parser.h"

// Вывод на экран состояния клиента
void print_client_state(id_t id, pid_t pid, state_t state) {
    unsigned int vol = number_of_states(state);
    char buf[16];
    if (vol == 1) {
        switch (state) {
            case OFF:
                sprintf(buf, "OFF");
                break;
            case RED:
                sprintf(buf, "RED");
                break;
            case YELLOW:
                sprintf(buf, "YELLOW");
                break;
            case GREEN:
                sprintf(buf, "GREEN");
                break;
            case ALL:
                sprintf(buf, "ALL");
                break;
            default:
                fprintf(stderr, "State error: invalid client state\n");
                exit(EXIT_FAILURE);
        }
        printf("[%d] Client #%d is in state %s\n", pid, id, buf);
    }
    else if (vol > 1) {
        printf("[%d] Client #%d is in %d states: ", pid, id, vol);
        for (char i = 0; i < 3; i++) {
            if (state & (1 << i)) {
                switch (i) {
                    case 0:
                        printf("RED, ");
                        break;
                    case 1:
                        printf("YELLOW, ");
                        break;
                    case 2:
                        printf("GREEN, ");
                        break;
                    default:
                        fprintf(stderr, "State error: unknown state\n");
                        exit(EXIT_FAILURE);
                }
            }
        }
        printf("\b \b\b \b\n");
    }
    else {
        fprintf(stderr, "State error: invalid combination of states\n");
        exit(EXIT_FAILURE);
    }
}

// Вычисление количества состояний клиента
unsigned int number_of_states(state_t state) {
    if (state == OFF)
        return 1;
    if (state == ALL)
        return 1;
    unsigned int res = 0;
    for (char i = 0; i < 3; i++)
        if (state & (1 << i))
            res++;
    return res;
}

// Проверка, является ли состояние допустимым
bool isvalidstate(state_t state) {
    return (state == OFF) || (state == RED) || (state == YELLOW) || (state == GREEN) || 
           (state == (RED|YELLOW)) || (state == (RED|GREEN)) || (state == (YELLOW|GREEN)) ||
           (state == ALL);
}

// Вывод на экран всех доступных состояний для клиента
void print_all_valiable_states(void) {
    printf("OFF, RED, YELLOW, GREEN, RED | YELLOW, RED | GREEN, YELLOW | GREEN, ALL\n");
}

// Перевод строки в состояние
state_t str2state(char *str, pid_t pid) {
    parser_set_state(str);
    state_t out = OFF;
    char *tmp = strtok(str, " |");
    while (tmp != NULL) {
        if (!strcmp(tmp, "OFF"))
            out |= OFF;
        else if (!strcmp(tmp, "RED"))
            out |= RED;
        else if (!strcmp(tmp, "YELLOW"))
            out |= YELLOW;
        else if (!strcmp(tmp, "GREEN"))
            out |= GREEN;
        else if (!strcmp(tmp, "ALL"))
            out |= ALL;
        else {
            printf("[%d] State error: unknown state\n", pid);
            printf("[%d] Client state must be one of the following: ", pid);
            print_all_valiable_states();
            return -1;
        }
        tmp = strtok(NULL, " |");
    }
    return (isvalidstate(out)) ? out : -1;
}
