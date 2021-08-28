#include "network.h"

int Socket(int domain, int type, int protocol) {
    int out = socket(domain, type, protocol);
    if (out == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return out;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int status = bind(sockfd, addr, addrlen);
    if (status == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog) {
    int status = listen(sockfd, backlog);
    if (status == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int out;
    out = accept(sockfd, addr, addrlen);
    if (out == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return out;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int status;
    status = connect(sockfd, addr, addrlen);
    if (status == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void Inet_ntop(int af, const void *src, char *dst, socklen_t size) {
    inet_ntop(af, src, dst, size);
    if (dst == NULL) {
        perror("inet_ntop failed");
        exit(EXIT_FAILURE);
    }
}

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

ssize_t Read(int fd, void *buf, size_t count) {
    ssize_t bytes = read(fd, buf, count);
    if (bytes == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (bytes == 0)
        printf("EOF occured");
    return bytes;
}