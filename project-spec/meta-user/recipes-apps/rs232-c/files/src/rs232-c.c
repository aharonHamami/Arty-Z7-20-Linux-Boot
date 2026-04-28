#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "common.h"
#include "uart.h"
#include "recordings.h"

volatile sig_atomic_t is_running = 1;

static void handle_sigint(int sig) {
    is_running = 0; // stop the program
}

int main()
{
    signal(SIGINT, handle_sigint);

    char serial_buffer[255];

    int serial_fd = uart_init(UART_DEVICE);
    if(serial_fd < 0) {
        fprintf(stderr, "Error: couldn't open serial device.\n");
        return EXIT_FAILURE;
    }

    printf("Listening for serial connection at %s\n", UART_DEVICE);

    FILE *rec_f_ptr = recordings_new();
    if(rec_f_ptr == NULL) {
        fprintf(stderr, "Error: couldn't open a new recording file");
        return EXIT_FAILURE;
    }

    while (is_running)
    {
        // Read from UART

        int n_bytes_read = uart_read(serial_fd, serial_buffer, sizeof(serial_buffer), 1);

        if (n_bytes_read == 0)
        {
            fprintf(stderr, "serial_read: received an empty input\n");
            return EXIT_FAILURE;
        }
        else if (n_bytes_read < 0)
        {
            fprintf(stderr, "bridge: Failed to read from uart\n");
            return EXIT_FAILURE;
        }

        printf("> %s\n", serial_buffer);

        // Write to a file

        printf("printed:\n");
        puts(serial_buffer);

        if(fputs(serial_buffer, rec_f_ptr) == EOF) {
            perror("fputs");
            fprintf(stderr, "Error: couldn't write to the recording file");
        }
        fflush(rec_f_ptr);
    }

    printf("Clean resources");
    uart_close(serial_fd);
    recordings_close(rec_f_ptr);
    
    printf("Finished");
    return 0;
}
