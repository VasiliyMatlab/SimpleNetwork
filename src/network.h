#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

// Максимальное количество подключаемых клиентов
#define BACKLOG 5
// Номер порта
#define PORT 3453

// Переопределение системного вызова socket(2)
int Socket(int domain, int type, int protocol);
// Переопределение системного вызова bind(2)
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// Переопределение системного вызова listen(2)
void Listen(int sockfd, int backlog);
// Переопределение системного вызова accept(2)
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// Переопределение системного вызова connect(2)
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// Переопределение системного вызова inet_ntop(3)
void Inet_ntop(int af, const void *src, char *dst, socklen_t size);
// Переопределение системного вызова inet_pton(3)
void Inet_pton(int af, const char *src, void *dst);
// Переопределение системного вызова recvfrom(2)
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, 
                 struct sockaddr *src_addr, socklen_t *addrlen);
// Переопределение системного вызова sendto(2)
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen);

#endif