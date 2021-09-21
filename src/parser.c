#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
                    "invalid settings in attr.\n");
                exit(EINVAL);
            case EPERM:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "no permission to set the scheduling policy "
                    "and parameters specified in attr.\n");
                exit(EPERM);
            default:
                fprintf(stderr, "Threads error: prthread_create failed: "
                    "unknown type of error.\n");
                exit(EXIT_FAILURE);
        }
    }
}

// Переопределение вызова pthread_join(2)
void Pthread_join(pthread_t thread, void **retval) {
    int status;
    status = pthread_join(thread, retval);
    if (status != 0) {
        switch (status) {
            case EDEADLK:
                fprintf(stderr, "Threads error: prthread_join failed: "
                    "a deadlock was detected "
                    "(e.g., two threads tried to join with each other); "
                    "or thread specifies the calling thread.\n");
                exit(EDEADLK);
            case EINVAL:
                fprintf(stderr, "Threads error: prthread_join failed: "
                    "thread is not a joinable thread.\nor\n");
                fprintf(stderr, "another thread is already waiting "
                    "to join with this thread.\n");
                exit(EINVAL);
            case ESRCH:
                fprintf(stderr, "Threads error: prthread_join failed: "
                    "no thread with the ID thread could be found.\n");
                exit(ESRCH);
            default:
                fprintf(stderr, "Threads error: prthread_join failed: "
                    "unknown type of error.\n");
                exit(EXIT_FAILURE);
        }
    }
}
