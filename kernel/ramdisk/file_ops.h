#ifndef __kernel_ramdisk_file_ops_h__
#define __kernel_ramdisk_file_ops_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  int create_file(const char *filename, size_t size);
  int file_read(const char *filename, const void *buf, size_t len);
  int file_write(const char *filename, const void *buf, size_t len);
  void list_files();

#ifdef __cplusplus
}
#endif

#endif // __kernel_ramdisk_file_ops_h__
