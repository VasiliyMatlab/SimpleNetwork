#ifndef CONDITIONS_H
#define CONDITIONS_H

#define LOG "clients.dan"

typedef enum {
    OFF      = 0b0000,
    RED      = 0b0001,
    YELLOW   = 0b0010,
    GREEN    = 0b0100,
    ALL      = 0b0111
} States_t;

typedef unsigned char state_t;

void print_client_state(id_t id, pid_t pid, state_t state);
unsigned int number_of_states(state_t state);

#endif