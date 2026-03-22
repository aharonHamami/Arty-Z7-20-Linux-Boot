#include <iostream>
#include <thread>

#include "arty_sys/serial.hpp"
#include "arty_sys/net_tcp.hpp"

void run_serial_worker(int input_fd, char* buffer, size_t buffer_size) = delete;

void run_net_worker() = delete;

int run_bridge(int serial_fd, TcpServer* net_server) {
    // TODO: add a ring buffer for threading safety

    // TODO: right now it handles only one client. increase it to multiple clients later
	printf("Listening for connection on port %d...\n", net_server->port);
	fflush(stdout);
	int client_fd = tcp_server_accept(net_server);

	printf("Waiting for U-ART input...\n");

	char buffer[255];	// input buffer
	while(1) {
		int n_bytes_read = serial_read(serial_fd, buffer, sizeof(buffer) - 1);

		if(n_bytes_read == 0) {
			printf("serial_read 0 bytes");
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		if(n_bytes_read < 0) {
			perror("serial_read");
			return 1;
		}

		// null terminating the input string
		buffer[n_bytes_read] = '\0';

		printf("sending> %s\n", buffer);
		int n_bytes_sent = tcp_server_send(client_fd, buffer, n_bytes_read);
        if(n_bytes_sent < 0 && errno == EPIPE) {
            close(client_fd);
            printf("[[ Client disconnected ]]\n");
            printf("Waiting for another connection...\n");
            fflush(stdout);
            client_fd = tcp_server_accept(net_server);
            printf("Connection established\n");
        }
	}

    return 0;
}
