#ifndef INTERNAL_H
#define INTERNAL_H

// Переопределение системного вызова creat(2)
int Creat(const char *pathname, mode_t mode);
// Переопределение системного вызова close(2)
void Close(int fd);
// Переопределение системного вызова read(2)
ssize_t Read(int fd, void *buf, size_t count);
// Переопределение системного вызова remove(3)
void Remove(const char *pathname);

#endif