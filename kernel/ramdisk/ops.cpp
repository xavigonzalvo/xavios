#include "ramdisk.h"
#include "string.h" // for memcpy

extern "C"
{
  void ramdisk_read(void *buf, size_t offset, size_t len)
  {
    // Check if the read would go beyond the end of the RAM disk
    if (offset + len > RAMDISK_SIZE)
    {
      // Handle error, e.g., print an error message or return an error code
      return;
    }

    // Copy the data from the RAM disk to the buffer
    memcpy(buf, &ramdisk[offset], len);
  }

  void ramdisk_write(const void *buf, size_t offset, size_t len)
  {
    // Check if the write would go beyond the end of the RAM disk
    if (offset + len > RAMDISK_SIZE)
    {
      // Handle error, e.g., print an error message or return an error code
      return;
    }

    // Copy the data from the buffer to the RAM disk
    memcpy(&ramdisk[offset], buf, len);
  }

} // extern "C"
