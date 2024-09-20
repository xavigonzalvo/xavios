#include "kernel_arm64/uart.h"

extern void kernel_main(void);

// Add this function for debugging
// __attribute__((used)) void debug_print(const char *message)
// {
//   uart_init();
//   uart_puts(message);
//   while (1)
//   {
//   }
// }

// QEMU debug exit device
#define QEMU_DEBUG_EXIT_ADDR 0x09000000

// Write to the QEMU debug exit device
static inline void qemu_exit(unsigned int exit_code)
{
  mmio_write(QEMU_DEBUG_EXIT_ADDR, exit_code);
}

void kernel_main()
{
  // debug_print("Debug: Entered kernel_main\n");
  uart_init();
  uart_puts("Init!\n");
  uart_puts("Hello from AArch64 kernel!\n");
  uart_puts("Press 'q' to exit.\n");

  while (1)
  {
    if (uart_getc_ready())
    {
      char c = uart_getc();
      if (c == 'q' || c == 'Q')
      {
        uart_puts("Exiting...\n");
        qemu_exit(0);
      }
      else
      {
        uart_putc(c);
      }
    }
  }
}

void bb() {}
