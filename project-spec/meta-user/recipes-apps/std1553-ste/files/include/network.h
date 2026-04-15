// Handles UDP communication, allowing network clients to register by sending packets.

#pragma once

#include <netinet/in.h>
#include <stdint.h>

/**
 * Start the network layer and listen on the given UDP port.
 * @param port listening port
 * @return socket file descriptor
 */
int net_init(int port);

/**
 * Receive a packet from a connected client.
 * @param sock_fd socket file descriptor
 * @param buf destination buffer for received data
 * @param size maximum number of bytes to read
 * @param client_addr caller-provided address structure for sender info
 * @return number of bytes received or negative error code
 */
int net_recv(int sock_fd, void *buf, int size, struct sockaddr_in *client_addr);

/**
 * Send a packet to the specified client.
 * @param sock_fd socket file descriptor
 * @param buf source buffer of data to send
 * @param size number of bytes to send
 * @param client_addr destination client address
 * @return number of bytes sent or negative error code
 */
int net_send(int sock_fd, const void *buf, int size, const struct sockaddr_in *client_addr);

/**
 * Shut down the network socket and free resources.
 * @param sock_fd socket file descriptor
 */
void net_close(int sock_fd);
