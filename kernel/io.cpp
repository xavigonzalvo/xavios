#include "io.h"

extern "C"
{

  uint8_t inb(uint16_t port)
  {
    uint8_t result;
    __asm__ volatile("inb %1, %0"
                     : "=a"(result) // output
                     : "Nd"(port)); // input
    return result;
  }

  void outb(uint16_t port, uint8_t data)
  {
    __asm__ volatile("outb %0, %1" ::"a"(data), "Nd"(port)); // input only, no output
  }

  uint16_t inw(uint16_t port)
  {
    uint16_t result;
    __asm__ volatile("inw %1, %0"
                     : "=a"(result) // output
                     : "Nd"(port)); // input
    return result;
  }

  void outw(uint16_t port, uint16_t data)
  {
    __asm__ volatile("outw %0, %1" ::"a"(data), "Nd"(port)); // input only, no output
  }

} // extern "C"
