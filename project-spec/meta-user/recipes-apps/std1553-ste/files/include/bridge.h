#pragma once

#include "sys/epoll.h"

#define BRIDGE_BUFFER_SIZE 255
#define BRIDGE_MAX_EVENTS 5

typedef struct {
    int sock_fd;
    int uart_fd;
    char buffer[BRIDGE_BUFFER_SIZE]; // sock <-> serial buffer

    // event loop properties
    int epoll_fd;
    struct epoll_event active_events[BRIDGE_MAX_EVENTS];
} bridge_t;

/**
 * Initialize a new bridge
 */
int bridge_init(bridge_t* bridge);

/**
 * Blocking function that listening to events with file descriptors related to the bridge
 * 
 * @param bridge bridge instance
 * @param is_running pointer to a number indicating to stop running when the value is 0
 * @return 0 if finished successfully, negative otherwise 
 */
int bridge_run(bridge_t* bridge, volatile int* is_running);

/**
 * Clear bridge resources
 */
void bridge_close(bridge_t* bridge);
