// Manages the mapping between MIL-STD-1553 Remote Terminal (RT) addresses and network clients.

#pragma once

#include <stdint.h>
#include <netinet/in.h>

#define MAX_RT 32

void router_init(void);
void router_register_client(uint8_t rt_addr, const struct sockaddr_in *client_addr);
const struct sockaddr_in *router_get_client(uint8_t rt_addr);
uint8_t router_parse_rt(uint16_t command_word);
