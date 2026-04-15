// Manages the UART interface for 1553 traffic.

#pragma once

#include <stdint.h>

int uart_init(const char *device);
int uart_read(int uart_fd, uint16_t *buf, int count);
int uart_write(int uart_fd, const uint16_t *buf, int count);
int uart_close(int uart_fd);
