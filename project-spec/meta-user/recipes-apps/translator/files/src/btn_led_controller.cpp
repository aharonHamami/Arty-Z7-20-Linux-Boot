#include "btn_led_controller.h"

#define GPIO_SIZE 0x10000
#define BTN_CHANNEL 1
#define LED_CHANNEL 2

BtnLedController::BtnLedController(const char *device_path): uio(device_path, GPIO_SIZE)
{
    // Configure I/O directions
    uio.setChannelDirection(BTN_CHANNEL, 0xFFFFFFFF);   // set buttons as input
    uio.setChannelDirection(LED_CHANNEL, 0x0);          // set LEDs as output
}

void BtnLedController::setAllLeds(uint32_t value)
{
    uio.writeChannel(LED_CHANNEL, value);
}

void BtnLedController::setLed(int led_no, int value)
{
    uio.writeIoPin(LED_CHANNEL ,led_no, value);
}

uint32_t BtnLedController::getAllLeds()
{
    return uio.readChannel(LED_CHANNEL);
}

int BtnLedController::getLed(int led_no)
{
    return uio.readIoPin(LED_CHANNEL, led_no);
}

void BtnLedController::reset()
{
    setAllLeds(0);
}
