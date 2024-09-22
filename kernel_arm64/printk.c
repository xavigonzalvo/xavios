#include "kernel_arm64/uart.h"

// Color codes
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

// Function to write an integer
void uart_puti(int num)
{
  char buf[16];
  int i = 0;
  int sign = num < 0 ? -1 : 1;

  if (num == 0)
  {
    uart_putc('0');
    return;
  }

  if (sign < 0)
  {
    uart_putc('-');
    num = -num;
  }

  while (num > 0)
  {
    buf[i++] = '0' + (num % 10);
    num /= 10;
  }

  while (--i >= 0)
  {
    uart_putc(buf[i]);
  }
}

// Function to write a hexadecimal number
void uart_puth(unsigned int num)
{
  const char hex_chars[] = "0123456789ABCDEF";
  char buf[9];
  int i = 0;

  do
  {
    buf[i++] = hex_chars[num & 0xF];
    num >>= 4;
  } while (num > 0);

  uart_puts("0x");
  while (--i >= 0)
  {
    uart_putc(buf[i]);
  }
}

// Function to handle color codes
void handle_color(char color)
{
  switch (color)
  {
  case 'r':
    uart_puts(COLOR_RED);
    break;
  case 'g':
    uart_puts(COLOR_GREEN);
    break;
  case 'y':
    uart_puts(COLOR_YELLOW);
    break;
  case 'b':
    uart_puts(COLOR_BLUE);
    break;
  case 'm':
    uart_puts(COLOR_MAGENTA);
    break;
  case 'c':
    uart_puts(COLOR_CYAN);
    break;
  case 'w':
    uart_puts(COLOR_WHITE);
    break;
  case '0':
    uart_puts(COLOR_RESET);
    break;
  default:
    break;
  }
}

// Custom printf function with color support
void printk(const char *format, ...)
{
  __builtin_va_list args;
  __builtin_va_start(args, format);

  while (*format)
  {
    if (*format == '%')
    {
      format++;
      switch (*format)
      {
      case 's':
        uart_puts(__builtin_va_arg(args, char *));
        break;
      case 'd':
        uart_puti(__builtin_va_arg(args, long long));
        break;
      case 'x':
        uart_puth(__builtin_va_arg(args, unsigned long long));
        break;
      case 'c':
        uart_putc((char)__builtin_va_arg(args, int));
        break;
      case '%':
        uart_putc('%');
        break;
      default:
        uart_putc('%');
        uart_putc(*format);
        break;
      }
    }
    else if (*format == '{' && *(format + 1) == '{')
    {
      format += 2; // Skip '{{'
      if (*format && *(format + 1) == '}' && *(format + 2) == '}')
      {
        handle_color(*format);
        format += 2; // Skip color code and '}}'
      }
      else
      {
        uart_puts("{{"); // Print '{{' if it's not a valid color code
      }
    }
    else
    {
      uart_putc(*format);
    }
    format++;
  }

  __builtin_va_end(args);
}