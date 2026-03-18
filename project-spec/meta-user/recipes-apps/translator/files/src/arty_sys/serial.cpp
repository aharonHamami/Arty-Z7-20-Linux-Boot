#include "serial.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define BAUD_RATE B9600

static int serial_configure(serial_fd fd)
{
    int rc;             // Error return code
    struct termios tty_conf; // device

    // Configure a serial port
    // Save the state of the file definition into tty
    rc = tcgetattr(fd, &tty_conf);
    if (rc != 0)
    {
        perror("tcgetattr");
        return -1;
    }

    // Input and Output baud rate
    cfsetispeed(&tty_conf, BAUD_RATE);
    cfsetospeed(&tty_conf, BAUD_RATE);

    // Configuration
    tty_conf.c_cflag &= ~PARENB;        // PARENBN = Parity Enable (Disabled)
    tty_conf.c_cflag &= ~CSTOPB;        // CSTOPB = C Stop Bits (one stop bit), when enabled - 2 stop bits
    tty_conf.c_cflag &= ~CSIZE;         // CSIZE = Character Size (Clear CS Flags for the next line).
    tty_conf.c_cflag |= CS8;            // CS = Character Size (8 bits per chunk)
    tty_conf.c_cflag |= CREAD | CLOCAL; // C = Channel (enable Read and Write)

    // Enable raw mode (non-canonical mode)
    // In short - let the data flow without terminal interruptions (like pressing the Backspace key).
    tty_conf.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Enable NON CANONICAL Mode for Serial Port Comm
    tty_conf.c_iflag &= ~(IXON | IXOFF | IXANY);         // Turn OFF software based flow control (XON/XOFF)
    tty_conf.c_oflag &= ~OPOST;

    printf("Timing is SET");
    tty_conf.c_cc[VMIN] = 1;        // return from 'read' only after getting at least 1 byte
    tty_conf.c_cc[VTIME] = 0;      // return from 'read' after VTIME*0.1 sec passed without input

    // Apply the settings on tty
    return tcsetattr(fd, TCSANOW, &tty_conf); // (TCSANOW = apply now)
}

serial_fd serial_open(const char *device_path)
{
    // O_RDWR - Read and Write
    // O_NOCTTY - prevents the port from becoming the controlling terminal
    int fd = open(device_path, O_RDWR | O_NOCTTY);
    if(fd < 0) {
        return -1;
    }

    int rc = serial_configure(fd);
    if(rc != 0) {
        return -1;
    }

    return fd;
}

int serial_close(serial_fd fd) {
    return close(fd);
}

ssize_t serial_write(serial_fd fd, void* buffer, int buffer_len) {
    return write(fd, buffer, buffer_len);
}

ssize_t serial_read(serial_fd fd, void* buffer, int buffer_len) {
    return read(fd, buffer, buffer_len);
}
