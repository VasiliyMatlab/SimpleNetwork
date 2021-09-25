#ifndef INTERNAL_H
#define INTERNAL_H

// Переопределение системного вызова close(2)
void Close(int fd);
// Переопределение вызова usleep(3)
void Usleep(unsigned int usec);

#endif