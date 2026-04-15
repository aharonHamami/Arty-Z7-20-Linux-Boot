#include "router.h"
#include <string.h>
#include <stddef.h>

static struct sockaddr_in rt_table[MAX_RT];
static int rt_active[MAX_RT];

void router_init(void) {
    memset(rt_active, 0, sizeof(rt_active));
}

void router_register_client(uint8_t rt_addr, const struct sockaddr_in *client_addr) {
    if (rt_addr < MAX_RT) {
        rt_table[rt_addr] = *client_addr;
        rt_active[rt_addr] = 1;
    }
}

const struct sockaddr_in *router_get_client(uint8_t rt_addr) {
    if (rt_addr < MAX_RT && rt_active[rt_addr]) {
        return &rt_table[rt_addr];
    }
    return NULL;
}

uint8_t router_parse_rt(uint16_t command_word) {
    // MIL-STD-1553 Command Word: Bits 15-11 are RT Address
    return (command_word >> 11) & 0x1F;
}
