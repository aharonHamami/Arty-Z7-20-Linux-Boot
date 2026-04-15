#include "network.h"

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int net_init(int port) {
    struct sockaddr_in server_addr;

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        sock_fd = -1;
        return -1;
    }

    return sock_fd;
}

int net_recv(int sock_fd, void *buf, int size, struct sockaddr_in *client_addr) {
    socklen_t len = sizeof(struct sockaddr_in);
    return recvfrom(sock_fd, buf, size, 0, (struct sockaddr *)client_addr, &len);
}

int net_send(int sock_fd, const void *buf, int size, const struct sockaddr_in *client_addr) {
    return sendto(sock_fd, buf, size, 0, (const struct sockaddr *)client_addr, sizeof(struct sockaddr_in));
}

void net_close(int sock_fd) {
    if (sock_fd >= 0) {
        close(sock_fd);
        sock_fd = -1;
    }
}
