#ifndef __kernel_terminal_h
#define __kernel_terminal_h

#ifdef __cplusplus
extern "C"
{
#endif

  void terminal_initialize(void);

  void terminal_writestring(const char *data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __kernel_terminal_h
