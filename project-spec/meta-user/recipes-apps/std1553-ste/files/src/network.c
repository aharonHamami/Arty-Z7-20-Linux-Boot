#include "network.h"

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int udp_init(int port) {
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

int udp_recv(int sock_fd, void *buffer, int buff_size, struct sockaddr_in *client_addr) {
    socklen_t len = sizeof(struct sockaddr_in);
    return recvfrom(sock_fd, buffer, buff_size, 0, (struct sockaddr *)client_addr, &len);
}

int udp_send(int sock_fd, const void *buffer, int buff_size, const struct sockaddr_in *client_addr) {
    return sendto(sock_fd, buffer, buff_size, 0, (const struct sockaddr *)client_addr, sizeof(struct sockaddr_in));
}

void udp_close(int sock_fd) {
    if (sock_fd >= 0) {
        close(sock_fd);
        sock_fd = -1;
    }
}
