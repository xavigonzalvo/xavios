// QEMU virt machine UART0 base address
#define UART0_BASE 0x09000000

// UART registers
#define UART_DR (UART0_BASE + 0x00)
#define UART_FR (UART0_BASE + 0x18)
#define UART_IBRD (UART0_BASE + 0x24)
#define UART_FBRD (UART0_BASE + 0x28)
#define UART_LCRH (UART0_BASE + 0x2C)
#define UART_CR (UART0_BASE + 0x30)

// QEMU debug exit device
#define QEMU_DEBUG_EXIT_ADDR 0x09000000

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

// Function to send a character to UART
void uart_putc(char c)
{
  // Wait for UART to become ready to transmit
  while (mmio_read(UART_FR) & (1 << 5))
  {
  }

  // Write the character to the UART data register
  mmio_write(UART_DR, c);
}

// Function to print a string via UART
void uart_puts(const char *str)
{
  while (*str)
  {
    uart_putc(*str++);
  }
}

// ... [Previous UART code remains the same]

// Function to check if there's any input available
int uart_getc_ready()
{
  return !(mmio_read(UART_FR) & (1 << 4));
}

// Function to read a character from UART
char uart_getc()
{
  // Wait for UART to have received something
  while (mmio_read(UART_FR) & (1 << 4))
  {
  }

  // Read the character from the UART data register
  return mmio_read(UART_DR) & 0xFF;
}

// Write to the QEMU debug exit device
static inline void qemu_exit(unsigned int exit_code)
{
  mmio_write(QEMU_DEBUG_EXIT_ADDR, exit_code);
}

void kernel_main()
{
  uart_init(); // Initialize UART
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
        uart_putc(c); // Echo the character
      }
    }
  }
}
