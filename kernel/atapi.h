#ifndef __kernel_atapi_h
#define __kernel_atapi_h

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  bool atapi_read_sector(uint32_t lba, uint16_t *buffer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __kernel_atapi_h
