#include <iostream>
#include <thread>

#include "arty_sys/serial.hpp"
#include "arty_sys/net_tcp.hpp"

static int handle_serial_input(int input_fd, char* buffer, size_t buffer_size) {
    int n_bytes_read = serial_read(input_fd, buffer, buffer_size);

	if(n_bytes_read == 0) {
		fprintf(stderr, "serial_read: received an empty input");
	}

	if(n_bytes_read < 0) {
		perror("serial_read");
		return -1;
	}

	// null terminating the input string
	buffer[n_bytes_read] = '\0';
	return n_bytes_read;
};

static void handle_ethernet_output(TcpServer* server, char* buffer, size_t buffer_size) {
	printf("sending> %s\n", buffer);

	for(int i=0; i<TCP_MAX_CLIENTS; i++) {
		int client_fd = server->clients.fds[i];

		if(client_fd == NO_CLIENT_FD) continue;

		int n_sent = tcp_server_send(client_fd, buffer, buffer_size);
		if(n_sent < 0 && errno == EPIPE) {
			close(client_fd);
			printf("[[ Client %d disconnected ]]\n", client_fd);
		}
	}
}

int run_bridge(int serial_fd, TcpServer* net_server) {
    // TODO: add a ring buffer for threading safety

	printf("Listening for connection on port %d...\n", net_server->port);
	printf("Waiting for U-ART input...\n");

	char buffer[255];	// serial -> ethernet buffer

	int rc; // general return code
	while(1) {
		// wait for clients accept requests OR wait for U-ART input
		fd_set readfs;
		FD_ZERO(&readfs);
		
		FD_SET(serial_fd, &readfs);
		FD_SET(net_server->fd, &readfs);

		int max_fd = serial_fd > net_server->fd ? serial_fd : net_server->fd;

		if(select(max_fd + 1, &readfs, NULL, NULL, NULL) < 0) {
			perror("select");
			return -1;
		}
				
		if(FD_ISSET(net_server->fd, &readfs)) {
			// Client wants to join
			int client_fd = tcp_server_accept(net_server);
			if(client_fd < 0) {
				fprintf(stderr, "Something went wrong, the server could not receive a new client\n");
			} else {
				printf("[A new client have joined - %d]\n", client_fd);
				fflush(stdout);
			}
		}

		if(FD_ISSET(serial_fd, &readfs)) { 
			// Serial data arrived
			int n_read = handle_serial_input(serial_fd, buffer, sizeof(buffer) - 1);
			if(rc < 0) return -1;
			
			handle_ethernet_output(net_server, buffer, n_read);
		}
	}

    return 0;
}
