#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdexcept>
#include <string>

#include "uio_helper.hpp"

UioGpio::UioGpio(const char* device_path, size_t size) : map_size(size)
{
   fd = open(device_path, O_RDWR);
   if(fd < 0) {
      fprintf(stderr, "Failed to open UIO device at '%s'", device_path);
      throw std::runtime_error("Failed to open UIO device");
   }

   regs = (uint32_t*) mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   if(regs == MAP_FAILED) {
      /*fprintf(stderr, "Failed to open UIO device at '%s'", device_path);  */
      throw std::runtime_error("Failed to open UIO device");
   }
}

UioGpio::~UioGpio() {
   munmap((void*)regs, map_size);
   close(fd);
}

/*
Define channel direction for all 32 registers at once.
The mask defines which registers are intended for input and which ones for output.
For example: a mask of 00001111 will set the lowest 4 registers to input and all 
the rest registers will be treated as output registers.

channel - gpio channel, can be 1 or 2
input_mask - registers input mask
*/
void UioGpio::setChannelDirection(int channel, uint32_t input_mask)
{
   if (channel == 1)
      regs[CH1_TRI] = input_mask;
   else if(channel == 2)
      regs[CH2_TRI] = input_mask;
   else
      throw std::runtime_error("An unknown channel was given (only 1 or 2 are supported)");
}

/*
Write the value for all 32 registers at once.

channel - gpio channel, can be 1 or 2
value - 32-bit representation of the registers value (those who were configured as 'input')
*/
void UioGpio::writeChannel(int channel, uint32_t value)
{
   if (channel == 1)
      regs[CH1_DATA] = value;
   else
      regs[CH2_DATA] = value;
}

/*
Read the value from all 32 registers at once.

channel - gpio channel, can be 1 or 2

returns: 32-bit representation of the registers value (those who were configured as 'output')
*/
uint32_t UioGpio::readChannel(int channel)
{
   if (channel == 1)
      return regs[CH1_DATA];
   else
      return regs[CH2_DATA];
}

void UioGpio::enableInterrupts()
{
   regs[GIER] = 0x80000000;
   regs[IP_IER] = 1;
}

void UioGpio::clearInterrupt()
{
   regs[IP_ISR] = 1;
}

void UioGpio::waitForInterrupt()
{
   uint32_t irq_count;
   read(fd, &irq_count, sizeof(irq_count));
}
