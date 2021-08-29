#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>

// Максимальное количество подключаемых клиентов
#define BACKLOG 5

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

#endif