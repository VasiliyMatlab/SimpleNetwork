#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    OFF      = 0b000,
    RED      = 0b001,
    YELLOW   = 0b010,
    GREEN    = 0b100,
    CHECKING = 0b111
} States_t;
typedef unsigned char state_t;

void print_client_state(id_t id, pid_t pid, state_t state);

#endif