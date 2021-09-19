#ifndef INTERNAL_H
#define INTERNAL_H

#define _GNU_SOURCE

// Переопределение системного вызова creat(2)
int Creat(const char *pathname, mode_t mode);
// Переопределение системного вызова close(2)
void Close(int fd);
// Переопределение системного вызова open(2)
int Open(const char *pathname, int flags);
// Переопределение системного вызова read(2)
ssize_t Read(int fd, void *buf, size_t count);
// Переопределение системного вызова remove(3)
void Remove(const char *pathname);
// Переопределение системного вызова write(2)
ssize_t Write(int fd, const void *buf, size_t count);
// Переопределение системного вызова clone(2)
pid_t Clone(int (*fn)(void *), void *stack, int flags, void *arg);

#endif