#include "uart.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE B9600

int uart_init(const char *device) {
    struct termios tty; // tty configuration

    int uart_fd = open(device, O_RDWR | O_NOCTTY | O_SYNC);
    if (uart_fd < 0) {
        perror("open");
        return -1;
    }

    // Configure a serial port
    // Save the state of the file definition into tty
    if (tcgetattr(uart_fd, &tty) != 0) {
        perror("tcgetattr");
        close(uart_fd);
        uart_fd = -1;
        return -1;
    }

    // Input and Output baud rate
    cfsetospeed(&tty, BAUD_RATE);
    cfsetispeed(&tty, BAUD_RATE);

    // Configuration
    tty.c_cflag &= ~PARENB;        // PARENBN = Parity Enable (Disabled)
    tty.c_cflag &= ~CSTOPB;        // CSTOPB = C Stop Bits (one stop bit), when enabled - 2 stop bits
    tty.c_cflag &= ~CSIZE;         // CSIZE = Character Size (Clear CS Flags for the next line).
    tty.c_cflag |= CS8;            // CS = Character Size (8 bits per chunk)
    tty.c_cflag |= CREAD | CLOCAL; // C = Channel (enable Read and Write)

    // Enable raw mode (non-canonical mode)
    // In short - let the data flow without terminal interruptions (like pressing the Backspace key).
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Enable NON CANONICAL Mode for Serial Port Comm
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Turn OFF software based flow control (XON/XOFF)
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1;     // return from 'read' only after getting at least 1 byte
    tty.c_cc[VTIME] = 0;    // return from 'read' after VTIME*0.1 sec passed without input

    if (tcsetattr(uart_fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        close(uart_fd);
        uart_fd = -1;
        return -1;
    }

    return uart_fd;
}

int uart_read(int uart_fd, uint16_t *buf, int count) {
    if (uart_fd < 0) {
        fprintf(stderr, "Error: Uart device was not initialized");
        return -1;
    }
    return read(uart_fd, buf, count * sizeof(uint16_t));
}

int uart_write(int uart_fd, const uint16_t *buf, int count) {
    if (uart_fd < 0) {
        fprintf(stderr, "Error: Uart device was not initialized");
        return -1;
    }
    return write(uart_fd, buf, count * sizeof(uint16_t));
}

int uart_close(int uart_fd) {
    if (uart_fd >= 0) {
        int rc = close(uart_fd);
        uart_fd = -1;
        return rc;
    }
    
    return -1;
}
