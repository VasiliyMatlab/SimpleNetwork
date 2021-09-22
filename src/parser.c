#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "parser.h"

// Парсер приема сообщений от клиентов
cmd_in parser_in(char *str) {
    char str_copy[BUFSIZ];
    strcpy(str_copy, str);
    cmd_in out = IN_NONE;
    char *tmp = strtok(str_copy, " ");
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
    if (out == IN_NONE) {
        fprintf(stderr, "Parser error: unknown message type\n");
        exit(EXIT_FAILURE);
    }
    return out;
}

// Парсер выдачи сообщений клиентам
cmd_out parser_out(char *str) {
    char str_copy[BUFSIZ];
    strcpy(str_copy, str);
    cmd_out out = OUT_NONE;
    char *tmp = strtok(str_copy, " ");
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
    if (out == OUT_NONE) {
        fprintf(stderr, "Parser error: unknown message type\n");
        exit(EXIT_FAILURE);
    }
    return out;
}

// Парсер принимаемых команд от пользователя
cmd_terminal parser_terminal(char *str) {
    char str_copy[BUFSIZ];
    strcpy(str_copy, str);
    cmd_terminal out = TERM_NONE;
    char *tmp = strtok(str_copy, " ");
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
    return out;
}

// Переопределение вызова pthread_create(3)
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg) {
    int status;
    status = pthread_create(thread, attr, start_routine, arg);
    if (status != 0) {
        switch (status) {
            case EAGAIN:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "insufficient resources to create another thread.\nor\n");
                fprintf(stderr, "a system-imposed limit on the number "
                    " of threads was encountered.\n");
                exit(EAGAIN);
            case EINVAL:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "invalid settings in \e[4mattr\e[0m.\n");
                exit(EINVAL);
            case EPERM:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "no permission to set the scheduling policy "
                    "and parameters specified in \e[4mattr\e[0m.\n");
                exit(EPERM);
            default:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "unknown type of error.\n");
                exit(EXIT_FAILURE);
        }
    }
}

// Переопределение вызова pthread_cancel(3)
void Pthread_cancel(pthread_t thread) {
    int status = pthread_cancel(thread);
    if (status != 0) {
        fprintf(stderr, "Threads error: prthread_cancel failed: "
            "no thread with the ID \e[4mthread\e[0m could be found.\n");
        exit(ESRCH);
    }
}
