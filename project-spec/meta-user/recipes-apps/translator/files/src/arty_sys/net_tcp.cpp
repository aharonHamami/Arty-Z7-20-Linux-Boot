#include "net_tcp.hpp"
#include "net_tcp_clients.hpp"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int tcp_server_init(TcpServer* server, int port) {
    // open a socket (IPv4, TCP)
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server->fd == -1 || server->fd == 0) {
        perror("socket");
        return -1;
    }

    // Set the SO_REUSEADDR option (allow binding to an address that was already in use)
    // This option helps when restarting the server too quickly. 
    int opt = 1; // setsockopt gets the value as (void*) so we provide the value by reference
    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt (SO_REUSEADDR)");
        return -1;
    }

    // configure server address
    server->address.sin_family = AF_INET;          // IPv4 protocol
    server->address.sin_addr.s_addr = INADDR_ANY;  // Listen to all network interfaces
    server->address.sin_port = htons(port);        // Server port
    server->port = port;

    tcp_clients_init(&server->clients);

    if(bind(server->fd, (struct sockaddr*)&server->address, sizeof(server->address)) < 0) {
        perror("bind");
        return -1;
    }

    return 0;
}

int tcp_server_close(TcpServer* server) {
    return close(server->fd);
}

int tcp_server_listen(TcpServer* server) {
    if(listen(server->fd, TCP_MAX_CLIENTS) < 0) { // allow max 3 pending connection requests queued
        perror("listen");
        return -1;
    }

    return 0;
}

int tcp_server_accept(TcpServer* server) {
    unsigned int address_size = sizeof(server->address);

    int client_socket_fd = accept(
        server->fd,
        (struct sockaddr*)&server->address,
        &address_size
    );

    if(!tcp_clients_can_add(&server->clients)) {
        fprintf(stderr, "tcp_server_accept: too many clients have joined. closing the last client\n");
        close(client_socket_fd);
        return -1;
    }

    if(client_socket_fd < 0) {
        perror("accept");
        return -1;
    }

    if(tcp_clients_add(&server->clients, client_socket_fd) < 0) {
        fprintf(stderr, "tcp_clients_add: a client could not be added\n");
        return -1;
    }
    
    return client_socket_fd;
}

ssize_t tcp_server_send(int client_fd, char* buffer, size_t buffer_length) {
    // MSG_NOSIGNAL - prevent a SIGPIPE signal, only return an EPIPE error
    ssize_t n_sent = send(client_fd, buffer, buffer_length, MSG_NOSIGNAL);

    if(n_sent < 0) {
        perror("send");
        return -1;
    }

    return n_sent;
}

int tcp_server_close_client(TcpServer* server, int client_fd) {
    close(client_fd);
    return tcp_clients_remove(&server->clients, client_fd);
}
