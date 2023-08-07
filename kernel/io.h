#ifndef __kernel_io_h
#define __kernel_io_h

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  uint8_t inb(uint16_t port);
  void outb(uint16_t port, uint8_t data);
  uint16_t inw(uint16_t port);
  void outw(uint16_t port, uint16_t data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __kernel_io_h
