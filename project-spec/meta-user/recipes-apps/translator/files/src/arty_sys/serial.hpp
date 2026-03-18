#pragma once

#include <stdio.h>

typedef int serial_fd; // file description

/**
 * Open a new serial connection
 * 
 * @return a file descriptor to the serial connection. -1 if there is an error.
 */
serial_fd serial_open(const char* device_path);

/**
 * close a serial connection
 * 
 * @param fd file descriptor of the serial connection.
 * @return status code (0 if ok).
 */
int serial_close(serial_fd fd);

/**
 * Write to a serial device
 * 
 * @param fd file descriptor of the serial connection.
 * @param buffer pointer to the output data buffer.
 * @param buffer_len buffer length.
 * @return number of bytes written, or -1 if fails.
 */
ssize_t serial_write(serial_fd fd, void* buffer, int buffer_len);

/**
 * Read from a serial device
 * 
 * @param fd file descriptor of the serial connection.
 * @param buffer pointer to the input data buffer.
 * @param buffer_len bytes count to read.
 * @return number of bytes read, -1 for errors or 0 for EOF.
 */
ssize_t serial_read(serial_fd fd, void* buffer, int buffer_len);
