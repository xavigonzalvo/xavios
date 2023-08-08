#ifndef __kernel_ramdisk_ops_h__
#define __kernel_ramdisk_ops_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  void ramdisk_read(void *buf, size_t offset, size_t len);
  void ramdisk_write(const void *buf, size_t offset, size_t len);

#ifdef __cplusplus
}
#endif

#endif // __kernel_ramdisk_ops_h__
