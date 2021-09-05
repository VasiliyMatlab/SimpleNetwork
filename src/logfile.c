#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "logfile.h"

// Переопределение системного вызова ftok(3)
key_t Ftok(const char *pathname, int proj_id) {
    key_t key = ftok(pathname, proj_id);
    if (key == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }
    return key;
}

// Переопределение системного вызова shmget(2)
int Shmget(key_t key, size_t size, int shmflg) {
    int fd = shmget(key, size, shmflg);
    if (fd == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// Переопределение системного вызова shmat(2)
void *Shmat(int shmid, const void *shmaddr, int shmflg) {
    void *ptr = shmat(shmid, shmaddr, shmflg);
    if (ptr == (void *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Переопределение системного вызова shmdt(2)
int Shmdt(const void *shmaddr) {
    int status = shmdt(shmaddr);
    if (status == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }
    return status;
}