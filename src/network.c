// Содержание:
//      Заголовочные файлы
//      Глобальные функции

//----------------------------Заголовочные файлы-------------------------------
// Пользовательские заголовочные файлы
#include "network.h"

//----------------------------Глобальные функции-------------------------------
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
