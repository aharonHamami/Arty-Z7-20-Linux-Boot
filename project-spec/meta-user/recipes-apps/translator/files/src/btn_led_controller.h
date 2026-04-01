#pragma once

#include "arty_sys/uio_gpio.hpp"

class BtnLedController {
    private:
    UioGpio uio;

    public:
    /**
     * Initialize the GPIO device for button and LED control.
     */
    BtnLedController();
    
    /**
     * Get gpio file descriptor
     */
    int getFD();

    /**
     * Get the state of an individual LED.
     * @param led_no LED number to read (starting from 0)
     * @return LED state
     */
    int getLed(int led_no);


    /**
     * Read the current state of all LEDs.
     * @return LED state as a 32-bit value
     */
    uint32_t getAllLeds();

    /**
     * Read the current state of all LEDs.
     * @return 4-bit mask representing the buttons state, or -1 in an error
     */
    int waitForButtonPress(); // TODO: add a maximum timeout

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
     * Turns off all the LEDs
     */
    void reset();
};
