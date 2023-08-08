#include "terminal.h"
#include "sprintf.h"

// Your basic va_list implementation
// typedef char *va_list;
// #define va_start(ap, v) (ap = (va_list)&v + sizeof(v))
// #define va_arg(ap, t) (*(t *)((ap += sizeof(t)) - sizeof(t)))
// #define va_end(ap) (ap = (va_list)0)

// Buffer size for formatted strings.
#define PRINTF_BUFFER_SIZE 1024

extern "C"
{

  void simple_printf(const char *format, ...)
  {
    char buffer[PRINTF_BUFFER_SIZE];

    va_list args;
    va_start(args, format);

    // Use your simple_sprintf to format the string
    simple_vsprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    // Write the formatted string to the terminal
    terminal_writestring(buffer);
  }

} // extern "C"
