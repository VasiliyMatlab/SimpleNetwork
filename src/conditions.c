#include "conditions.h"

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