#pragma once

#include "arty_sys/uio_gpio.hpp"

class BtnLedController {
    private:
    UioGpio uio;

    public:
    /**
     * Initialize the GPIO device for button and LED control.
     * @param device_path Path to the UIO GPIO device
     */
    BtnLedController(const char* device_path);

    /**
     * Set all LEDs to a specific state.
     * @param value The LED state value to write
     */
    void setAllLeds(uint32_t value);

    /**
     * Set an individual LED on or off.
     * @param led_no LED number to control (starting from 0)
     * @param value LED state (0 for off, 1 for on)
     */
    void setLed(int led_no, int value);

    /**
     * Read the current state of all LEDs.
     * @return LED state as a 32-bit value
     */
    uint32_t getAllLeds();

    /**
     * Get the state of an individual LED.
     * @param led_no LED number to read (starting from 0)
     * @return LED state
     */
    int getLed(int led_no);

    /**
     * Turns off all the LEDs
     */
    void reset();
};
