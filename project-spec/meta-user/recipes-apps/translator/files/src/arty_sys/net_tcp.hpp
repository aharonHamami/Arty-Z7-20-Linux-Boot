#pragma once

#include "net_tcp_clients.hpp"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TCP_MAX_CLIENTS 4

typedef struct
{
    int fd;
    struct sockaddr_in address;
    int port;
    TcpClients clients;
} TcpServer;

/**
 * Creates a TCP server
 * This is a blocking function that can handle a single TCP connection at a time
 * 
 * @param server pointer to the server instance
 * @param port listening port
 * @return 0 in success, -1 in failure
 */
int tcp_server_init(TcpServer* server, int port = 3000);

/**
 * Close a server TCP socket
 * 
 * @param server pointer to the server instance
 * @return status code (0 if ok).
 */
int tcp_server_close(TcpServer* server);

/**
 * Allow clients to start a connection
 * 
 * @param server pointer to the server instance
 * @return 0 in success, -1 in failure
 */
int tcp_server_listen(TcpServer* server);

/**
 * Accept a single connection from a client
 * 
 * @param server pointer to the server instance
 * @return client connection's file descriptor, or -1 in failure
 */
int tcp_server_accept(TcpServer* server);

/**
 * Send data to the client.
 * 
 * @param client_fd client connection file descriptor.
 * @param buffer data buffer to send.
 * @param buffer_length buffer size in bytes.
 * @return Number of bytes sent, or -1 in a failure.
 */
ssize_t tcp_server_send(int client_fd, char* buffer, size_t buffer_length);
