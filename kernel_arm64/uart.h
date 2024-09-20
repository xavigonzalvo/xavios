#ifndef __KERNEL_ARM64_UART_H__
#define __KERNEL_ARM64_UART_H__

// Function to write to a memory-mapped register
static inline void mmio_write(unsigned long reg, unsigned int val)
{
  *(volatile unsigned int *)reg = val;
}

// Function to read from a memory-mapped register
static inline unsigned int mmio_read(unsigned long reg)
{
  return *(volatile unsigned int *)reg;
}

// Initialize UART
void uart_init();

// Function to send a character to UART
void uart_putc(char c);

// Function to print a string via UART
void uart_puts(const char *str);

// Function to check if there's any input available
int uart_getc_ready();

// Function to read a character from UART
char uart_getc();

#endif
