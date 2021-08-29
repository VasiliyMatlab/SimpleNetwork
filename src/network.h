#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>

#define BACKLOG 5

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Inet_ntop(int af, const void *src, char *dst, socklen_t size);
void Inet_pton(int af, const char *src, void *dst);

#endif