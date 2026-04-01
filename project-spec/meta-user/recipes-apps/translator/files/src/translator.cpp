#include <iostream>
#include <string.h>
#include <chrono>

#include "arty_sys/serial.hpp"
#include "arty_sys/net_tcp.hpp"
#include "btn_led_controller.h"
#include "bridge.hpp"

#define SERIAL_F_PATH "/dev/ttyPS0"
#define TCP_PORT 3000

void show_help(const char* prog_name) {
	printf("Description:\n");
	printf("UART Serial to Ethernet communication bridge\n");
	printf("\n");
	printf("Usage: sudo %s <serial-address> <destination-ip> [<destination-port>]\n", prog_name);
}

int run_translator() {
	// Setup serial connection
	int serial_fd = serial_open(SERIAL_F_PATH);
	if(serial_fd < 0) {
		perror("serial_open");
		return 1;
	}
	printf("Opened a serial connection at %s\n", SERIAL_F_PATH);

	// Setup TCP server
	TcpServer server;
	tcp_server_init(&server, TCP_PORT);
	tcp_server_listen(&server);

	// Setup gpio for 4 buttons and leds
	BtnLedController btn_led_io;
	btn_led_io.reset();
	
	int return_code = run_bridge_event_loop(serial_fd, &server, &btn_led_io);

	// clean resources
	serial_close(serial_fd);
	tcp_server_close(&server);

	return return_code;
}

int main(int argc, char *argv[])
{
	const char* prog_name = argv[0];

	// proccess options
	for(int i=1; i<argc; i++) {
		const char* opt = argv[i];

		if(strcmp(opt, "-h") == 0 || strcmp(opt, "--help") == 0) {
			show_help(prog_name);
			return 0;
		}
	}

	return run_translator();
}
