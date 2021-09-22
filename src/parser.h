#ifndef PARSER_H
#define PARSER_H

#include <pthread.h>

// Команды на прием сообщений от клиента
typedef enum {
    IN_NONE     = 0x0,  // отсутствие команды
    IN_LAUNCH   = 0x1,  // клиент запущен
    IN_STATE    = 0x2,  // прием состояния клиента
    IN_SHUTDOWN = 0x3   // отключение клиента
} cmd_in;

// Команды на отправку сообщения клиенту
typedef enum {
    OUT_NONE     = 0x0, // отсутствие команды
    OUT_DENY     = 0x1, // отклонить запуск клиента
    OUT_CONN     = 0x2, // подтвердить подключение клиента
    OUT_GETSTATE = 0x3, // выдать состояние клиента
    OUT_SETSTATE = 0x4, // установить состояние клиента
    OUT_SHUTDOWN = 0x5  // отключить клиента
} cmd_out;

//
typedef enum {
    TERM_NONE       = 0x0,  // отсутствие команды
    TERM_LAUNCH     = 0x1,  // проверить наличие клиента
    TERM_GETSTATE   = 0x2,  // выдать состояние клиента
    TERM_SETSTATE   = 0x3,  // установить состояние клиента
    TERM_SHDWN_CLNT = 0x4,  // отключить клиента
    TERM_SHDWN_SERV = 0x5   // выключить сервер
} cmd_terminal;

// Парсер приема сообщений от клиентов
cmd_in parser_in(char *str);
// Парсер выдачи сообщений клиентам
cmd_out parser_out(char *str);
// Парсер принимаемых команд от пользователя
cmd_terminal parser_terminal(char *str);
// Переопределение вызова pthread_create(3)
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
// Переопределение вызова pthread_cancel(3)
void Pthread_cancel(pthread_t thread);

#endif