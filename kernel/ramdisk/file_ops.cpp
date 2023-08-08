#include <stddef.h>

#include "string.h"
#include "ramdisk/ramdisk.h"
#include "ramdisk/ops.h"
#include "printf.h"

size_t next_offset = 0;

extern "C"
{

  int create_file(const char *filename, size_t size)
  {
    // Find an empty slot in the file table
    for (int i = 0; i < MAX_FILES; i++)
    {
      if (file_table[i].size == 0)
      {
        // Check if there's enough space in the RAM disk
        if (next_offset + size > RAMDISK_SIZE)
        {
          // Handle error
          return -1;
        }

        // Populate the file metadata
        strncpy(file_table[i].filename, filename, MAX_FILENAME);
        file_table[i].size = size;
        file_table[i].offset = next_offset;

        // Update the next available offset
        next_offset += size;

        return 0; // Success
      }
    }

    // File table is full
    return -1;
  }

  int file_read(const char *filename, void *buf, size_t len)
  {
    // Find the file in the file table
    for (int i = 0; i < MAX_FILES; i++)
    {
      if (strncmp(file_table[i].filename, filename, MAX_FILENAME) == 0)
      {
        // Check if the read would go beyond the end of the file
        if (len > file_table[i].size)
        {
          // Handle error
          return -1;
        }

        // Read the data from the RAM disk
        ramdisk_read(buf, file_table[i].offset, len);
        return 0; // Success
      }
    }

    // File not found
    return -1;
  }

  int file_write(const char *filename, const void *buf, size_t len)
  {
    // Find the file in the file table
    for (int i = 0; i < MAX_FILES; i++)
    {
      if (strncmp(file_table[i].filename, filename, MAX_FILENAME) == 0)
      {
        // Check if the write would go beyond the end of the file
        if (len > file_table[i].size)
        {
          // Handle error
          return -1;
        }

        // Read the data from the RAM disk
        ramdisk_write(buf, file_table[i].offset, len);
        return 0; // Success
      }
    }

    // File not found
    return -1;
  }

  void list_files()
  {
    simple_printf("Files:\n");

    // Iterate through the file table and print the details of each file
    for (int i = 0; i < MAX_FILES; i++)
    {
      if (file_table[i].size > 0)
      {
        simple_printf("Filename: %s, Size: %d bytes, Offset: %d\n",
                      file_table[i].filename, file_table[i].size, file_table[i].offset);
      }
    }
  }

} // extern "C"
