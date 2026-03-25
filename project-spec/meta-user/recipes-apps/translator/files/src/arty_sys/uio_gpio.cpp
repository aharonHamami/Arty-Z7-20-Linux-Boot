#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdexcept>
#include <string>

#include "uio_gpio.hpp"

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

void UioGpio::setChannelDirection(int channel, uint32_t input_mask)
{
   if (channel == 1)
      regs[CH1_TRI] = input_mask;
   else if(channel == 2)
      regs[CH2_TRI] = input_mask;
   else
      throw std::runtime_error("An unknown channel was given (only 1 or 2 are supported)");
}

void UioGpio::writeChannel(int channel, uint32_t value)
{
   if (channel == 1)
      regs[CH1_DATA] = value;
   else
      regs[CH2_DATA] = value;
}

void UioGpio::writeIoPin(int channel, int pin_no, int value)
{
   uint32_t channel_state = readChannel(channel);
   uint32_t pin_bitmask = 1 << pin_no;
   
   if(value) {
      channel_state |= pin_bitmask;    // enable bit 
   }else {
      channel_state &= ~pin_bitmask;   // disable bit
   }

   writeChannel(channel, channel_state);
}

uint32_t UioGpio::readChannel(int channel)
{
   if (channel == 1)
      return regs[CH1_DATA];
   else
      return regs[CH2_DATA];
}

int UioGpio::readIoPin(int channel, int pin_no)
{
   uint32_t pin_bitmask = 1 << pin_no;
   uint32_t channel_state = readChannel(channel);

   return (channel_state & pin_bitmask) == 0;
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
