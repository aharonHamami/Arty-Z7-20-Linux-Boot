#include "net_tcp_clients.hpp"

#include <stdlib.h>
#include <stdio.h>

static int tcp_clients_find_index(const TcpClients *clients, int client_fd)
{
    for(int i=0; i<TCP_MAX_CLIENTS; i++) {
        if(clients->fds[i] == client_fd) {
            return i;
        }
    }

    return -1;
}

void tcp_clients_init(TcpClients* clients) {
    for(int i=0; i<TCP_MAX_CLIENTS; i++) {
        clients->fds[i] = NO_CLIENT_FD;
    }

    clients->n_connected = 0; // no clients are connected yet
}

int tcp_clients_add(TcpClients* clients, int client_fd)
{
    int new_client_i = tcp_clients_find_index(clients, NO_CLIENT_FD);
    
    for(int i=0; i<TCP_MAX_CLIENTS; i++) {
        if(clients->fds[i] == client_fd) {          // client already exists
            return i;
        }
        else if(clients->fds[i] == NO_CLIENT_FD) {  // there is an empty spot available
            clients->fds[new_client_i] = client_fd;
            clients->n_connected++;
            return new_client_i;
        }
    }

    return -1; // no spot was found
}

int tcp_clients_can_add(const TcpClients *clients)
{
    return clients->n_connected < TCP_MAX_CLIENTS;
}

int tcp_clients_remove(TcpClients *clients, int client_fd)
{
    int remove_client_i = tcp_clients_find_index(clients, client_fd);
    if(remove_client_i < 0) {
        fprintf(stderr, "tcp_clients_remove: client not found\n");
        return -1;
    }

    clients->fds[remove_client_i] = NO_CLIENT_FD;
    clients->n_connected--;

    return 0;
}
