#include "uart.h"

// QEMU virt machine UART0 base address
#define UART0_BASE 0x09000000

// UART registers
#define UART_DR (UART0_BASE + 0x00)
#define UART_FR (UART0_BASE + 0x18)
#define UART_IBRD (UART0_BASE + 0x24)
#define UART_FBRD (UART0_BASE + 0x28)
#define UART_LCRH (UART0_BASE + 0x2C)
#define UART_CR (UART0_BASE + 0x30)

void uart_init()
{
  // Disable UART before configuration
  mmio_write(UART_CR, 0);

  // Set integer & fractional part of baud rate
  // Assuming 48MHz UART clock and 115200 baud rate
  mmio_write(UART_IBRD, 26);
  mmio_write(UART_FBRD, 3);

  // Enable FIFO & 8 bit data transmission (1 stop bit, no parity)
  mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  // Enable UART, receive & transfer part of UART
  mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(char c)
{
  // Wait for UART to become ready to transmit
  while (mmio_read(UART_FR) & (1 << 5))
  {
  }

  // Write the character to the UART data register
  mmio_write(UART_DR, c);
}

void uart_puts(const char *str)
{
  while (*str)
  {
    uart_putc(*str++);
  }
}

int uart_getc_ready()
{
  return !(mmio_read(UART_FR) & (1 << 4));
}

char uart_getc()
{
  // Wait for UART to have received something
  while (mmio_read(UART_FR) & (1 << 4))
  {
  }

  // Read the character from the UART data register
  return mmio_read(UART_DR) & 0xFF;
}

void uart_print_hex(unsigned long value)
{
  const char hex_chars[] = "0123456789ABCDEF";
  char buffer[sizeof(unsigned long) * 2 + 3]; // '0x' prefix + 16 hex digits + null terminator
  int index = sizeof(buffer) - 1;

  // Null-terminate the string
  buffer[index] = '\0';

  // Convert to hex, starting from the least significant digit
  do
  {
    index--;
    buffer[index] = hex_chars[value & 0xF];
    value >>= 4;
  } while (value != 0 && index > 2);

  // Add '0x' prefix
  buffer[--index] = 'x';
  buffer[--index] = '0';

  // Print the hex string
  for (const char *p = &buffer[index]; *p; p++)
  {
    uart_putc(*p);
  }
}

void log_el(unsigned int el)
{
  uart_puts("Current Exception Level (EL): ");

  // Convert EL number to a character
  char el_str[2];
  el_str[0] = '0' + el; // Convert integer to character
  el_str[1] = '\0';

  // Log the EL mode
  uart_puts(el_str);
  uart_puts("\n");
}
