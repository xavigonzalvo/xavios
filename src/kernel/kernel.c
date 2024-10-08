#include "uart.h"
#include "printk.h"
#include "types.h"
#include "fault_handler.h"

extern void kernel_main(void);

// Debugging function.
__attribute__((used)) void debug_print(const char *message)
{
  uart_init();
  uart_puts(message);
  while (1)
  {
  }
}

// QEMU debug exit device
#define QEMU_DEBUG_EXIT_ADDR 0x09000000

// Write to the QEMU debug exit device
static inline void qemu_exit(unsigned int exit_code)
{
  mmio_write(QEMU_DEBUG_EXIT_ADDR, exit_code);
}

extern void uart_puts(const char *str);
extern void uart_print_hex(unsigned long value);

void print_stack_pointer()
{
  // Print current stack pointer
  unsigned long sp;
  asm volatile("mov %0, sp" : "=r"(sp));
  uart_puts("Current stack pointer: ");
  uart_print_hex(sp);
  uart_puts("\n");
}

extern void init_mmu(void);

void kernel_main()
{
  uart_init();
  uart_puts("Kernel started\n");
  uart_puts("Vector table initialized\n");
  uart_puts("MMU initialized\n");

  while (1)
  {
    uart_putc('.');
    for (volatile int i = 0; i < 1000000; i++)
    {
    } // Simple delay
  }
}

// void kernel_main()
// {
//   uart_init();
//   // printk("Initializing UART ...");
//   // printk("[{{g}}OK{{0}}]\n");
//   uart_putc('a');
//   uart_putc('b');
//   uart_putc('c');
//   uart_puts("hello\n");
//   uart_puts("hello2\n");
//   bb("Initial ");
//   bb("Initial2");

//   // printk("Initializing kernel\n");
//   // // set_vector_table();
//   // init_mmu();

//   // printk("exit 0\n");
//   // qemu_exit(0);

//   // printk("Probing memory ...");
//   // test_memory_fault();
//   // // struct memory_map map;
//   // // detect_memory(&map);
//   // printk("Press 'q' to exit.\n");

//   // while (1)
//   // {
//   //   if (uart_getc_ready())
//   //   {
//   //     char c = uart_getc();
//   //     if (c == 'q' || c == 'Q')
//   //     {
//   //       uart_puts("Exiting...\n");
//   //       qemu_exit(0);
//   //     }
//   //     else
//   //     {
//   //       uart_putc(c);
//   //     }
//   //   }
//   // }
// }
