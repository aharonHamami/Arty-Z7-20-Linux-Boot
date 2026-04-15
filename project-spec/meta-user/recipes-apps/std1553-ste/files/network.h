// Handles UDP communication, allowing network clients to register by sending packets.

#pragma once

#include <netinet/in.h>
#include <stdint.h>

int net_init(int port);
int net_recv(int sock_fd, void *buf, int size, struct sockaddr_in *client_addr);
int net_send(int sock_fd, const void *buf, int size, const struct sockaddr_in *client_addr);
void net_close(int sock_fd);
