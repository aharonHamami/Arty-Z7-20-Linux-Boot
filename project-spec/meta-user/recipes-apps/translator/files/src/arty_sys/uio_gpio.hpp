#pragma once

#include <cstdint>
#include <cstddef>

class UioGpio
{
    private:
    int fd;                     // gpio/uio file descriptor
    volatile uint32_t* regs;    // memory map to the uio fd content
    size_t map_size;

    enum Registers
    {
        CH1_DATA = 0,
        CH1_TRI  = 1,

        CH2_DATA = 2,
        CH2_TRI  = 3,

        GIER   = 0x11C / 4,
        IP_ISR = 0x120 / 4,
        IP_IER = 0x128 / 4
    };

    // prevent copying/duplication of the instance
    UioGpio(const UioGpio&) = delete;
    UioGpio& operator=(const UioGpio&) = delete;

    public:
    UioGpio(const char* device_path, const char* map_size_path);
    ~UioGpio();

    /**
     * Get gpio file descriptor
     */
    int getFD();

    /**
     * Define channel direction for all 32 pins at once.
     * The mask defines which registers are intended for input and which ones for output.
     * For example: a mask of 00001111 will set the lowest 4 registers to input and all 
     * the rest registers will be treated as output registers.
     * 
     * @param channel GPIO channel (can be 1 or 2)
     * @param input_mask Bitmask describing which pins are input
     */
    void setChannelDirection(int channel, uint32_t input_mask);

    /**
     * Write the value for all 32 pins at once.
     * 
     * Only pins configured as outputs will be affected.
     * 
     * @param channel GPIO channel (can be 1 or 2)
     * @param value Bitmask representing of the output pins value.
     */
    void writeChannel(int channel, uint32_t value);

    /**
     * Write the value for a single pin in the channel.
     * 
     * Only pins configured as outputs can be affected.
     * 
     * @param channel GPIO channel (can be 1 or 2).
     * @param pin_no pin number in the channel (starting from 0).
     * @param value boolean value of the ping state (0 or 1).
     */
    void writeIoPin(int channel, int pin_no, int value);

    /**
     * Read the current value from all 32 pins at once.
     * 
     * @param channel GPIO channel (can be 1 or 2)
     * @return Bitmask representation of the input pins state.
     */
    uint32_t readChannel(int channel);

    /**
     * Read the current value from a single pin in a channel.
     * 
     * @param channel GPIO channel (can be 1 or 2)
     * @param pin_no pin number in the channel (starting from 0).
     * @return the input pin state.
     */
    int readIoPin(int channel, int pin_no);

    /**
     * Enable AXI GPIO interrupts on the specified channel.
     * 
     * This method can be called only once after creating a new instance.
     */
    void enableInterrupts(int channel);

    /**
     * Block until a hardware interrupt occurs.
     * 
     * @note After the interrupt is dispatched and the user handles the interrupt the 'resetInterrupt' method must be called again to wait for the interrupt again 
     * 
     * @return interrupt count
     */
    int32_t waitForInterrupt();

    /**
     * Clear the interrupt status register.
     */
    void resetInterrupt();
};
