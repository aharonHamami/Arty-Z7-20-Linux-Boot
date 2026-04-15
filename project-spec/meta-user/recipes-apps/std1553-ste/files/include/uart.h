// Manages the UART interface for 1553 traffic.

#pragma once

#include <stdint.h>

/**
 * Open a UART device.
 * @param device device path
 * @return UART file descriptor
 */
int uart_init(const char *device);

/**
 * Read data from the UART interface.
 * @param uart_fd UART file descriptor
 * @param buf destination buffer for data words
 * @param count number of words to read
 * @return number of words read or negative error code
 */
int uart_read(int uart_fd, uint16_t *buf, int count);

/**
 * Send data to the UART interface.
 * @param uart_fd UART file descriptor
 * @param buf source buffer of data words
 * @param count number of words to send
 * @return number of words sent or negative error code
 */
int uart_write(int uart_fd, const uint16_t *buf, int count);

/**
 * Close UART connection.
 * @param uart_fd UART file descriptor
 * @return 0 in success, negative otherwise
 */
int uart_close(int uart_fd);
