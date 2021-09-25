// Содержание:
//      Заголовочные файлы
//      Глобальные функции

//----------------------------Заголовочные файлы-------------------------------
// Заголовочные файлы из стандартной библиотеки C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Пользовательские заголовочные файлы
#include "internal.h"

//----------------------------Глобальные функции-------------------------------
// Переопределение системного вызова close(2)
void Close(int fd) {
    int status = close(fd);
    if (status == -1) {
        perror("close failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение вызова usleep(3)
void Usleep(unsigned int usec) {
    int status = usleep(usec);
    if (status == -1) {
        perror("usleep failed");
        exit(EXIT_FAILURE);
    }
}
