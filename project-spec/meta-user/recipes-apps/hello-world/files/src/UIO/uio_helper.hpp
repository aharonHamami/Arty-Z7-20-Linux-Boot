#pragma once

#include <cstdint>
#include <cstddef>

class UioGpio
{
    private:
    int fd;
    volatile uint32_t* regs;
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
    UioGpio(const char* device_path, size_t size = 0x10000);
    ~UioGpio();

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
     * Read the current value from all 32 pins at once.
     * 
     * @param channel GPIO channel (can be 1 or 2)
     * @return Bitmask representation of the input pins state.
     */
    uint32_t readChannel(int channel);

    /**
     * Enable AXI GPIO interrupts.
     */
    void enableInterrupts();

    /**
     * Clear the interrupt status register.
     */
    void clearInterrupt();

    /**
     * Block until a hardware interrupt occurs.
     */
    void waitForInterrupt();
};
