#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>

#include "arty_sys/serial.hpp"

void show_help(const char* prog_name) {
	printf("UART Serial to Ethernet translator\n");
	printf("\n");
	printf("Description:\n");
	printf("\n");
	printf("Usage: %s <serial-address> <destination-ip> [<destination-port>]\n", prog_name);
}

int run_translator() {
	const char serial_path[] = "/dev/ttyPS0";
	serial_fd fd = serial_open(serial_path);
	if(fd < 0) {
		perror("serial_open");
		return 1;
	}

	printf("Waiting for signals from %s...\n", serial_path);

	char buffer[255];	// input buffer
	while(1) {
		const int nbytes = serial_read(fd, buffer, sizeof(buffer) - 1);

		if(nbytes == 0) {
			perror("serial_read 0 bytes");
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		if(nbytes < 0) {
			perror("serial_read");
			return 1;
		}

		// null terminating the input string
		buffer[nbytes] = '\0';

		printf("> %s\n", buffer);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	const char* prog_name = argv[0];

	// proccess options
	for(int i=1; i<argc; i++) {
		const char* opt = argv[argc];

		if(strcmp(opt, "-h") == 0 || strcmp(opt, "--help") == 0) {
			show_help(prog_name);
			return 0;
		}
	}

	return run_translator();
}
