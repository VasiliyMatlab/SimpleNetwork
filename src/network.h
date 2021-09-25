#ifndef NETWORK_H
#define NETWORK_H

// Заголовочные файлы из стандартной библиотеки C
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

// Максимальное количество подключаемых клиентов
#define BACKLOG 5
// Номер порта
#define PORT 3453

// Переопределение системного вызова socket(2)
int Socket(int domain, int type, int protocol);
// Переопределение системного вызова bind(2)
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// Переопределение системного вызова recvfrom(2)
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, 
                 struct sockaddr *src_addr, socklen_t *addrlen);
// Переопределение системного вызова sendto(2)
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen);

#endif