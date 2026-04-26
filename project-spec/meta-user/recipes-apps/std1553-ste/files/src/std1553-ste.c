#include <stdio.h>
#include <stdlib.h>

#include "bridge.h"

int main() {
    printf("Starting std1553-ste bridge...\n");

    // Initialization:

    bridge_t bridge;
    if(bridge_init(&bridge) < 0) {
        fprintf(stderr, "Failed to initialize bridge connection between ethernet and UART connections\n");
        return EXIT_FAILURE;
    }

    int always_running = 1;
    bridge_run(&bridge, &always_running);

    bridge_close(&bridge);

    return EXIT_SUCCESS;
}
