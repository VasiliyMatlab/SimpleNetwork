#include "network.h"

// Переопределение системного вызова socket(2)
int Socket(int domain, int type, int protocol) {
    int out = socket(domain, type, protocol);
    if (out == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return out;
}

// Переопределение системного вызова bind(2)
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int status = bind(sockfd, addr, addrlen);
    if (status == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова listen(2)
void Listen(int sockfd, int backlog) {
    int status = listen(sockfd, backlog);
    if (status == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова accept(2)
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int out;
    out = accept(sockfd, addr, addrlen);
    if (out == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return out;
}

// Переопределение системного вызова connect(2)
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int status;
    status = connect(sockfd, addr, addrlen);
    if (status == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова inet_ntop(3)
void Inet_ntop(int af, const void *src, char *dst, socklen_t size) {
    inet_ntop(af, src, dst, size);
    if (dst == NULL) {
        perror("inet_ntop failed");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова inet_pton(3)
void Inet_pton(int af, const char *src, void *dst) {
    int status = inet_pton(af, src, dst);
    if (status == -1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    if (status == 0) {
        fprintf(stderr, "inet_pton failed: src does not contain a character"
            " string representing a valid network address in the specified"
            " address family\n");
        exit(EXIT_FAILURE);
    }
}

// Переопределение системного вызова recvfrom(2)
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
    ssize_t bytes;
    bytes = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (bytes == -1) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }
    return bytes;
}

// Переопределение системного вызова sendto(2)
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen) {
    ssize_t bytes;
    bytes = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (bytes == -1) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
    return bytes;
}