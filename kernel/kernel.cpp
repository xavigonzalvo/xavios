#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "terminal.h"
#include "printf.h"
#include "sprintf.h"
#include "ramdisk/file_ops.h"

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void delay(int cycles)
{
  for (int i = 0; i < cycles; i++)
  {
    // This inner loop is purely to waste cycles for delay.
    for (volatile int j = 0; j < 10; j++)
    {
      // Do nothing, just waste some time.
    }
  }
}

char hexChars[] = "0123456789ABCDEF";

void byteToHex(uint8_t byte, char *out)
{
  out[0] = hexChars[(byte >> 4) & 0x0F]; // High nibble
  out[1] = hexChars[byte & 0x0F];        // Low nibble
}

void printHex(uint32_t val)
{
  char out[9]; // 8 chars for 32 bits + null terminator
  for (int i = 0; i < 4; i++)
  {
    byteToHex((val >> (8 * (3 - i))) & 0xFF, &out[i * 2]);
  }
  out[8] = '\0';      // Null-terminate the string
  simple_printf(out); // Replace with your own print function
}

// extern "C" void kernel_main(const void *multiboot_structure, uint32_t multiboot_magic)
extern "C" void kernel_main(uint32_t multiboot_magic, const void *mbinfo)
{
  /* Initialize terminal interface */
  terminal_initialize();
  if (multiboot_magic != 0x2BADB002)
  {
    // Not a valid multiboot header
    simple_printf("Invalid multiboot header\n");
    return;
  }
  simple_printf("Magic number is valid\n");

  for (int i = 0; i < 10; ++i)
  {
    simple_printf("int: %d\n", i);
  }

  char buffer[100];
  int value = 42;
  const char *str = "world";

  // Using simple_sprintf to print an integer and a string
  simple_sprintf(buffer, sizeof(buffer), "Hello, %s! The answer is %d\n", str, value);

  // Print the result
  terminal_writestring(buffer); // Output: "Hello, world! The answer is 42"

  create_file("test.txt", 100);
  create_file("test2.txt", 100);
  create_file("hola.txt", 1000);

  file_write("test.txt", buffer, sizeof(buffer));

  list_files();

  // Read file.
  simple_printf("\nReading file...\n");
  char read_buffer[100];
  file_read("test.txt", read_buffer, sizeof(read_buffer));
  terminal_writestring(read_buffer);

  // file system.

  // char buffer[100];
  // for (int i = 0; i < 100; i++)
  // {
  //   simple_sprintf(buffer, "%d\n", i);
  //   terminal_writestring(buffer);
  //   delay(1000000);
  // }

  // printHex(mbinfo->flags);

  // if (mbinfo->flags & 0x8)
  // { // Check if the MODS bit is set
  //   MultibootModule *mods = (MultibootModule *)mbinfo->mods_addr;

  //   // Assuming ISO is the first module. Adjust as per your setup.
  //   iso_start = mods[0].mod_start;
  //   iso_end = mods[0].mod_end;
  // }
  // else
  // {
  //   simple_printf("No modules loaded.\n");
  //   while (1)
  //   {
  //   }
  // }
}
