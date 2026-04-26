#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

#include "common.h"
#include "bridge.h"
#include "network.h"
#include "uart.h"

static void handle_serial_input(bridge_t *bridge)
{
    int uart_fd = bridge->uart_fd;

    // Read from UART

    int n_bytes_read = uart_read(uart_fd, bridge->buffer, BRIDGE_BUFFER_SIZE);

    if (n_bytes_read == 0)
    {
        fprintf(stderr, "serial_read: received an empty input\n");
        return;
    }
    else if (n_bytes_read < 0)
    {
        fprintf(stderr, "bridge: Failed to read from uart\n");
        return;
    }

    // terminating the received string with null
    bridge->buffer[n_bytes_read] = '\0';

    printf("serial> %s\n", bridge->buffer);

    // Output over UDP

    // TODO: send the UART data over udp
}

static void handle_udp_input(bridge_t *bridge)
{
    int sock_fd = bridge->sock_fd;

    // Read from UDP Socket

    struct sockaddr_in client_addr;
    int n_received = udp_recv(sock_fd, bridge->buffer, BRIDGE_BUFFER_SIZE, &client_addr);

    // terminating the received string with null
    bridge->buffer[n_received] = '\0';

    printf("udp> %s\n", bridge->buffer);

    // Output over UART

    // TODO: send the socket data over UART
}

static int register_events(bridge_t *bridge)
{
    bridge->epoll_fd = epoll_create1(0);
    if (bridge->epoll_fd == -1)
    {
        perror("bridge: epoll_create1");
        return -1;
    }

    // socket event
    struct epoll_event sock_event = {.data.fd = bridge->sock_fd, .events = EPOLLIN};
    if (epoll_ctl(bridge->epoll_fd, EPOLL_CTL_ADD, bridge->sock_fd, &sock_event) == -1)
    {
        perror("bridge: epoll_ctl: sock");
        return -1;
    }

    // uart event
    struct epoll_event uart_event = {.data.fd = bridge->uart_fd, .events = EPOLLIN};
    if (epoll_ctl(bridge->epoll_fd, EPOLL_CTL_ADD, bridge->uart_fd, &uart_event) == -1)
    {
        perror("bridge: epoll_ctl: uart");
        return -1;
    }

    return 0;
}

static void handle_event(bridge_t *bridge, const struct epoll_event *ev)
{
    printf("event detected\n");
    int source_fd = ev->data.fd;
    // int events = ev->events;

    if (source_fd == bridge->sock_fd)
    {
        handle_udp_input(bridge);
        return;
    }
    else if (source_fd == bridge->uart_fd)
    {
        handle_serial_input(bridge);
    }else {
        fprintf(stderr, "Error: an unknown event detected from device %d\n", source_fd);
    }
}

int bridge_init(bridge_t *bridge)
{
    int sock_fd = udp_init(UDP_PORT);
    if (sock_fd < 0)
    {
        fprintf(stderr, "Failed to initialize network\n");
        bridge_close(bridge);
        return -1;
    }
    printf("Listening for UDP packets on port %d\n", UDP_PORT);

    int uart_fd = uart_init(UART_DEVICE);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART\n");
        bridge_close(bridge);
        return -1;
    }
    printf("UART interface ready on %s (%d baud rate)\n", UART_DEVICE, UART_BAUD_RATE);

    bridge->sock_fd = sock_fd;
    bridge->uart_fd = uart_fd;

    if (register_events(bridge) < 0)
    {
        fprintf(stderr, "bridge: Failed to open event queue\n");
        bridge_close(bridge);
        return -1;
    }

    fflush(stdout); // Make sure stdout logs are finished

    return 0;
}

int bridge_run(bridge_t *bridge, volatile int *is_running)
{
    while (*is_running)
    {
        int n_events = epoll_wait(bridge->epoll_fd, bridge->active_events, BRIDGE_MAX_EVENTS, -1);

        if (n_events < 0)
        {
            if (errno == EINTR)
            { // system call was interrupted, try again
                continue;
            }

            perror("bridge: epoll_wait");
            return -1;
        }

        for (int i = 0; i < n_events; i++)
        {
            struct epoll_event ev = bridge->active_events[i];
            handle_event(bridge, &ev);
        }
    }

    return 0;
}

void bridge_close(bridge_t *bridge)
{
    if (bridge->sock_fd > 0)
        close(bridge->sock_fd);
    if (bridge->uart_fd > 0)
        close(bridge->uart_fd);
    if (bridge->epoll_fd > 0)
        close(bridge->epoll_fd);
}
