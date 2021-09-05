#ifndef LOGFILE_H
#define LOGFILE_H

#include <stdlib.h>

// Название лог-файла
#define LOG "clients.dan"

// Переопределение системного вызова ftok(3)
key_t Ftok(const char *pathname, int proj_id);
// Переопределение системного вызова shmget(2)
int Shmget(key_t key, size_t size, int shmflg);
// Переопределение системного вызова shmat(2)
void *Shmat(int shmid, const void *shmaddr, int shmflg);
// Переопределение системного вызова shmdt(2)
int Shmdt(const void *shmaddr);

#endif