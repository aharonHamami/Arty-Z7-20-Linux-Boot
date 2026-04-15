#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include "router.h"
#include "network.h"
#include "uart.h"

#define UDP_PORT 12345
#define UART_DEVICE "/dev/ttyPS0"
#define BUF_SIZE 1024

int main(int argc, char **argv) {
    uint16_t uart_buf[BUF_SIZE];
    char net_buf[BUF_SIZE * 2];
    struct sockaddr_in client_addr;
    fd_set read_fds;

    printf("Starting std1553-ste bridge...\n");

    router_init();

    int sock_fd = net_init(UDP_PORT);
    if (sock_fd < 0) {
        fprintf(stderr, "Failed to initialize network\n");
        return EXIT_FAILURE;
    }

    int uart_fd = uart_init(UART_DEVICE);
    if (uart_fd < 0) {
        fprintf(stderr, "Failed to initialize UART\n");
        net_close(uart_fd);
        return EXIT_FAILURE;
    }

    int max_fd = (sock_fd > uart_fd) ? sock_fd : uart_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(sock_fd, &read_fds);
        FD_SET(uart_fd, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        // Handle Network traffic
        if (FD_ISSET(sock_fd, &read_fds)) {
            int len = net_recv(sock_fd, net_buf, sizeof(net_buf), &client_addr);
            if (len > 0) {
                // Protocol: [RT_ADDR (1 byte)][DATA...]
                uint8_t rt_addr = (uint8_t)net_buf[0];
                router_register_client(rt_addr, &client_addr);
                
                // Forward the rest of the data to UART
                if (len > 1) {
                    uart_write(uart_fd, (uint16_t *)(net_buf + 1), (len - 1) / 2);
                }
            }
        }

        // Handle UART traffic
        if (FD_ISSET(uart_fd, &read_fds)) {
            int len = uart_read(uart_fd, uart_buf, BUF_SIZE);
            if (len > 0) {
                int word_count = len / sizeof(uint16_t);
                if (word_count > 0) {
                    uint8_t rt_addr = router_parse_rt(uart_buf[0]);
                    const struct sockaddr_in *target = router_get_client(rt_addr);
                    
                    if (target) {
                        net_send(sock_fd, uart_buf, len, target);
                    } else {
                        printf("Received data for unregistered RT: %d\n", rt_addr);
                    }
                }
            }
        }
    }

    uart_close(uart_fd);
    net_close(sock_fd);

    return EXIT_SUCCESS;
}
