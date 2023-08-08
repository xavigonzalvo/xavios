#ifndef KERNEL_SPRINTF_H

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

  int simple_vsprintf(char *buf, size_t size, const char *format, va_list ap);
  int simple_sprintf(char *buf, size_t size, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // KERNEL_SPRINTF_H
