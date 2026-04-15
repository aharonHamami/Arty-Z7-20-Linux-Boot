// Manages the mapping between MIL-STD-1553 Remote Terminal (RT) addresses and network clients.

#pragma once

#include <stdint.h>
#include <netinet/in.h>

#define MAX_RT 32

/**
 * Prepare the RT address routing table for use.
 */
void router_init(void);

/**
 * Associate a MIL-STD-1553 RT address with a network client.
 * @param rt_addr RT address to register
 * @param client_addr address of the network client
 */
void router_register_client(uint8_t rt_addr, const struct sockaddr_in *client_addr);

/**
 * Get the responsible client for the given RT address.
 * @param rt_addr RT address to query
 * @return pointer to the client address or NULL if unknown
 */
const struct sockaddr_in *router_get_client(uint8_t rt_addr);

/**
 * Extract the RT address from a 1553 command word.
 * @param command_word raw 1553 command word
 * @return decoded RT address
 */
uint8_t router_parse_rt(uint16_t command_word);
