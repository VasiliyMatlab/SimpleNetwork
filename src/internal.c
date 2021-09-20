#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "internal.h"

// Переопределение системного вызова creat(2)
int Creat(const char *pathname, mode_t mode) {
    int fd = creat(pathname, mode);
    if (fd == -1) {
        perror("creat failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// Переопределение системного вызова close(2)
void Close(int fd) {
    int status = close(fd);
    if (status == -1) {
        perror("close failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова open(2)
int Open(const char *pathname, int flags) {
    int fd = open(pathname, flags);
    if (fd == -1) {
        fprintf(stderr, "Server is not running\n");
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// Переопределение системного вызова read(2)
ssize_t Read(int fd, void *buf, size_t count) {
    ssize_t bytes = read(fd, buf, count);
    if (bytes == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (bytes == 0)
        printf("EOF occured\n");
    return bytes;
}

// Переопределение системного вызова remove(3)
void Remove(const char *pathname) {
    int status = remove(pathname);
    if (status == -1) {
        perror("remove failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова write(2)
ssize_t Write(int fd, const void *buf, size_t count) {
    ssize_t bytes = write(fd, buf, count);
    if (bytes == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }
    return bytes;
}