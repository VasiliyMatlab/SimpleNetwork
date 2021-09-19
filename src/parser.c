#include <stdio.h>
#include <string.h>
#include "parser.h"

// Парсер приема сообщений от клиентов
cmd_in parser_in(char *str, ssize_t len) {
    cmd_in out = IN_NONE;
    char *tmp = strtok(str, " ");
    while (tmp != NULL) {
        if (!strcmp(tmp, "launched")) {
            out = IN_LAUNCH;
            break;
        }
        if (!strcmp(tmp, "in")) {
            out = IN_STATE;
            break;
        }
        if (!strcmp(tmp, "shutdown")) {
            out = IN_SHUTDOWN;
            break;
        }
    }
    free(tmp);
    if (out == IN_NONE) {
        fprintf(stderr, "Parser error: unknown message type\n");
        exit(EXIT_FAILURE);
    }
    return out;
}