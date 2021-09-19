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
        tmp = strtok(NULL, " ");
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
    else if (!strcmp(tmp, "Client"))
        out = OUT_CONN;
    else if (!strcmp(tmp, "Get"))
        out = OUT_GETSTATE;
    else if (!strcmp(tmp, "Set"))
        out = OUT_SETSTATE;
    else if (!strcmp(tmp, "Shutdown"))
        out = OUT_SHUTDOWN;
    free(tmp);
    if (out == OUT_NONE) {
        fprintf(stderr, "Parser error: unknown message type\n");
        exit(EXIT_FAILURE);
    }
    return out;
}

// Парсер принимаемых команд от пользователя
cmd_terminal parser_terminal(char *str) {
    cmd_terminal out = TERM_NONE;
    char *tmp = strtok(str, " ");
    if (!strcmp(tmp, "Is"))
        out = TERM_LAUNCH;
    else if (!strcmp(tmp, "Get"))
        out = TERM_GETSTATE;
    else if (!strcmp(tmp, "Set"))
        out = TERM_SETSTATE;
    else {
        tmp = strtok(NULL, " ");
        if (!strcmp(tmp, "client"))
            out = TERM_SHDWN_CLNT;
        else if (!strcmp(tmp, "server"))
            out = TERM_SHDWN_SERV;
        else {
            fprintf(stderr, "Parser error: unknown command type\n");
            exit(EXIT_FAILURE);
        }
    }
    free(tmp);
    return out;
}