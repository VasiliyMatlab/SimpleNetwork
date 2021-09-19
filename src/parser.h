#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

// Команды на прием сообщений от клиента
typedef enum {
    NONE     = 0x0, // отсутствие команды
    LAUNCH   = 0x1, // клиент запущен
    STATE    = 0x2, // прием состояния клиента
    SHUTDOWN = 0x3  // отключение клиента
} cmd_in;

// Команды на отправку сообщения клиенту
typedef enum {
    NONE     = 0x0, // отсутствие команды
    DENY     = 0x1, // отклонить запуск клиента
    CONN     = 0x2, // подтвердить подключение клиента
    GETSTATE = 0x3, // выдать состояние клиента
    SETSTATE = 0x4, // установить состояние клиента
    SHUTDOWN = 0x5  // отключить клиента
} cmd_out;

//
typedef enum {
    NONE       = 0x0,   // отсутствие команды
    ISCLNT     = 0x1,   // проверить наличие клиента
    GETSTATE   = 0x2,   // выдать состояние клиента
    SETSTATE   = 0x3,   // установить состояние клиента
    SHDWN_CLNT = 0x4,   // отключить клиента
    SHDWN_SERV = 0x5    // выключить сервер
} cmd_terminal;

// Парсер приема сообщений от клиентов
cmd_in parser_in(char *str, ssize_t len);
// Парсер выдачи сообщений клиентам
cmd_out parser_out(char *str, ssize_t len);
// Парсер принимаемых команд от пользователя
cmd_terminal parser_terminal(char *str, ssize_t len);

#endif