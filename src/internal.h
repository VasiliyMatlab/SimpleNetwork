#ifndef INTERNAL_H
#define INTERNAL_H

int Creat(const char *pathname, mode_t mode);
void Close(int fd);
ssize_t Read(int fd, void *buf, size_t count);
void Remove(const char *pathname);

#endif