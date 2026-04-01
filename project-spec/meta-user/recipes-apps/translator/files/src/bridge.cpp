#include <iostream>
#include <thread>

#include "arty_sys/serial.hpp"
#include "arty_sys/net_tcp.hpp"
#include "btn_led_controller.h"

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
			tcp_server_close_client(server, client_fd);
			printf("[[ Client %d disconnected ]]\n", client_fd);
		}
	}
}

static void handle_button_press(BtnLedController* btn_led_io) {
	int pressed_btns_mask = btn_led_io->waitForButtonPress();
	printf("a button was pressed, got the value: %d\n", pressed_btns_mask);
}

int run_bridge_event_loop(int serial_fd, TcpServer* net_server, BtnLedController* btn_led_io) {
    printf("Listening for connection on port %d...\n", net_server->port);
	printf("Waiting for U-ART input...\n");

	char buffer[255];							// serial -> ethernet buffer
	int n_clients_connected = 0;				// save number of connected clients at each iteration
	int btn_led_fd = btn_led_io->getFD();

	while(1) {
		// wait for clients accept requests OR wait for U-ART input
		// TODO: fd_set and 'select' method is mandatory. use poll or epoll instead
		fd_set readfs;
		FD_ZERO(&readfs);
		
		FD_SET(serial_fd, &readfs);
		FD_SET(net_server->fd, &readfs);
		FD_SET(btn_led_fd, &readfs);

		int max_fd = serial_fd > net_server->fd ? serial_fd : net_server->fd;
		max_fd = max_fd > btn_led_fd ? max_fd : btn_led_fd;

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
			if(n_read < 0) return -1;
			
			handle_ethernet_output(net_server, buffer, n_read);
		}

		if(FD_ISSET(btn_led_fd, &readfs)) {
			handle_button_press(btn_led_io);
		}

		// indicate when clients connected/disconnected
		if(n_clients_connected != net_server->clients.n_connected) {
			n_clients_connected = net_server->clients.n_connected;
			printf("now %d are connected\n", (uint32_t)n_clients_connected);
			btn_led_io->setAllLeds(1 << (n_clients_connected - 1));
		}
	}

    return 0;
}
