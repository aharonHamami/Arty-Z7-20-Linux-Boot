#pragma once

#include <stdlib.h>

#define TCP_MAX_CLIENTS 4
#define NO_CLIENT_FD -1

typedef struct {
    int fds[TCP_MAX_CLIENTS]; // client sockets
    size_t n_connected;
} TcpClients;

/**
 * @brief Initializes the TcpClients structure.
 *
 * @param clients Pointer to the TcpClients structure to initialize.
 */
void tcp_clients_init(TcpClients* clients);

/**
 * @brief Adds a new client file descriptor to the TcpClients structure.
 *
 * Finds an available slot and assigns the client_fd.
 * Updates the number of connected clients.
 *
 * @param clients Pointer to the TcpClients structure.
 * @param client_fd The file descriptor of the client to add.
 * @return The index of the added client on success, -1 if no space available.
 */
int tcp_clients_add(TcpClients* clients, int client_fd);

/**
 * @brief Checks if a new client can be added to clients.
 *
 * @param clients Pointer to the TcpClients structure.
 * @return 1 if a client can be added, 0 otherwise.
 */
int tcp_clients_can_add(const TcpClients* clients);

/**
 * @brief Removes a client file descriptor from clients.
 *
 * @param clients Pointer to the TcpClients structure.
 * @param client_fd The file descriptor of the client to remove.
 * @return 0 on success, -1 if the client was not found.
 */
int tcp_clients_remove(TcpClients* clients, int client_fd);
