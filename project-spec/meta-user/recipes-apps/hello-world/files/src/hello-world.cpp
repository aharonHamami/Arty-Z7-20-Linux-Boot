#include <stdio.h>

#include "UIO/uio_helper.hpp"

#define GPIO_SIZE 0x10000
#define BTN_CHANNEL 1
#define LED_CHANNEL 2

void set_led_state(UioGpio uio, uint8_t led_no, int state) {

}

int main(int argc, char* argv[])
{
    UioGpio uio("/dev/uio0");
    char* command = nullptr;
    uint8_t selected_led = 1;

    /* buttons input */
    uio.setChannelDirection(BTN_CHANNEL, 0xFFFFFFFF);

    /* LEDs output */
    uio.setChannelDirection(LED_CHANNEL, 0x0);

    printf("Turn n the leds");

    /* Turn on LED1 and LED4 */
    uio.writeChannel(LED_CHANNEL, 0b1001);

    while (1);
}
