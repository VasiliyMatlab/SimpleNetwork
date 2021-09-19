#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Парсер приема сообщений от клиентов
cmd_in parser_in(char *str) {
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

// Парсер выдачи сообщений клиентам
cmd_out parser_out(char *str) {
    cmd_out out = OUT_NONE;
    char *tmp = strtok(str, " ");
    if (!strcmp(tmp, "Deny"))
        out = OUT_DENY;
    if (!strcmp(tmp, "Client"))
        out = OUT_CONN;
    if (!strcmp(tmp, "Get"))
        out = OUT_GETSTATE;
    if (!strcmp(tmp, "Set"))
        out = OUT_SETSTATE;
    if (!strcmp(tmp, "Shutdown"))
        out = OUT_SHUTDOWN;
    if (out == OUT_NONE) {
        fprintf(stderr, "Parser error: unknown message type\n");
        exit(EXIT_FAILURE);
    }
    return out;
}