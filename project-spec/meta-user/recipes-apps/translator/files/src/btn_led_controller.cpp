#include "btn_led_controller.h"

#define BTN_LED_GPIO_F_PATH "/dev/uio0"
#define GPIO_SIZE_F_PATH "/sys/class/uio/uio1/maps/map0/size"
/* #define GPIO_SIZE 0x10000 */
#define BTN_CHANNEL 1
#define LED_CHANNEL 2

BtnLedController::BtnLedController(): uio(BTN_LED_GPIO_F_PATH, GPIO_SIZE_F_PATH)
{
    // Configure I/O directions
    uio.setChannelDirection(BTN_CHANNEL, 0xFFFFFFFF);   // set buttons as input
    uio.setChannelDirection(LED_CHANNEL, 0x0);          // set LEDs as output

    // enable button press interrupts
    uio.enableInterrupts(BTN_CHANNEL);
}

int BtnLedController::getFD()
{
    return uio.getFD();
}

int BtnLedController::waitForButtonPress()
{
    uio.waitForInterrupt();

    const int pressed_btns_mask = uio.readChannel(BTN_CHANNEL);

    uio.resetInterrupt();

    return pressed_btns_mask;
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
