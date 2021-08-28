#include "conditions.h"

void print_client_state(id_t id, pid_t pid, state_t state) {
    char buf[16];
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
        case CHECKING:
            sprintf(buf, "CHECKING");
            break;
        default:
            fprintf(stderr, "State error: invalid client state\n");
            exit(EXIT_FAILURE);
    }
    printf("[%d] Client #%d is in state %s\n", pid, id, buf);
}