#include <iostream>
#include <thread>

#include "arty_sys/serial.hpp"
#include "arty_sys/net_tcp.hpp"

void run_serial_worker() {

}

void run_net_worker() {

}

int run_bridge(int serial_fd, TcpServer* net_server) {
    // TODO: add a ring buffer

    // TODO: right now it's only one client. increase it to multiple clients later
	printf("Listening for connection on port %d...\n", net_server->port);
	fflush(stdout);
	int client_fd = tcp_server_accept(net_server);

	printf("Waiting for U-ART input...\n");

	char buffer[255];	// input buffer
	while(1) {
		const int nbytes = serial_read(serial_fd, buffer, sizeof(buffer) - 1);

		if(nbytes == 0) {
			printf("serial_read 0 bytes");
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		if(nbytes < 0) {
			perror("serial_read");
			return 1;
		}

		// null terminating the input string
		buffer[nbytes] = '\0';

		printf("sending> %s\n", buffer);
		tcp_server_send(client_fd, buffer, nbytes);
	}
}
