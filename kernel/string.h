#ifndef __kernel_string_h__
#define __kernel_string_h__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  void *memcpy(void *dest, const void *src, size_t n);
  int strncmp(const char *str1, const char *str2, size_t n);
  void strncpy(char *dest, const char *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __kernel_string_h__
