/**
 * UIO documentation can be found here: https://www.kernel.org/doc/html/latest/driver-api/uio-howto.html
 */

#include "uio_gpio.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <assert.h>

/**
 * Get memory map size based on the information the UIO driver is giving.
 * 
 * @note Got inspiration from source: https://forum.digilent.com/topic/4750-how-to-detect-and-handle-uio-interrupt/
 * 
 * @param file_path sysfs file path (supposed to be located at /sys/class/uio/uioX/maps/mapX/size)
 */
static size_t get_mmap_size(const char* file_path) {
   // open the file that describes the memory range size that is based on the 
   // reg property of the node in the device tree 
   FILE * size_fp = fopen(file_path, "r"); 
   
   assert(size_fp != NULL && "Failed to open the uio size file");

   // get the size which is an ASCII string such as 0xXXXXXXXX and then be stop 
   // using the file 
   size_t mmap_size;
   int n_match = fscanf(size_fp, "0x%08X", &mmap_size);
   assert(n_match != EOF && "Failed to run fscanf to check memory map size for UIO");

   fclose(size_fp);

   return mmap_size; 
}

UioGpio::UioGpio(const char* device_path, const char* map_size_path)
{
   fd = open(device_path, O_RDWR);
   if(fd < 0) {
      perror("uio open");
      fprintf(stderr, "Failed to open UIO device at '%s'", device_path);
      throw std::runtime_error("Failed to open UIO device");
   }

   map_size = get_mmap_size(map_size_path);
   void* map_base = mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if(map_base == MAP_FAILED) {
      perror("uio mmap");
      close(fd);
      throw std::runtime_error("Failed to map UIO device memory");
   }
   
   regs = (uint32_t*) map_base;

   if(regs == MAP_FAILED) {
      /*fprintf(stderr, "Failed to open UIO device at '%s'", device_path);  */
      throw std::runtime_error("Failed to open UIO device");
   }
}

UioGpio::~UioGpio() {
   munmap((void*)regs, map_size);
   close(fd);
}

int UioGpio::getFD()
{
   return fd;
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

void UioGpio::enableInterrupts(int channel)
{
   regs[GIER] = 0x80000000;
   regs[IP_IER] = (uint32_t)channel; // enable interrupt on channel
}

int32_t UioGpio::waitForInterrupt()
{
   /*
   From the Docs:
   The signed 32 bit integer read is the interrupt count of your device.
   If the value is one more than the value you read the last time, everything is OK.
   If the difference is greater than one, you missed interrupts.
   */
  int32_t irq_count; // interrupt count
  ssize_t rc = read(fd, &irq_count, sizeof(irq_count));
  
  assert(rc > 0 && "UIO read error");
  
  return irq_count;
}

void UioGpio::resetInterrupt()
{
   // finish the current interrupt
   regs[IP_ISR] = 1;

   // reenable the interrupt again
   uint32_t enable = 1;
   int rc = write(fd, &enable, sizeof(enable));
   if(rc < 0) {
      perror("uio write");
      fprintf(stderr, "Failed to reenable the UIO interrupt");
   }
}
