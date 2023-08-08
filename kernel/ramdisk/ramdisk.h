#ifndef __kernel_ramdisk_ramdisk_h__
#define __kernel_ramdisk_ramdisk_h__

#include <stdint.h>

#define RAMDISK_SIZE 1024 * 1024 // 1 MB

static uint8_t ramdisk[RAMDISK_SIZE];

#define MAX_FILENAME 16
#define MAX_FILES 32

typedef struct
{
  char filename[MAX_FILENAME];
  size_t size;
  size_t offset;
} file_t;

static file_t file_table[MAX_FILES];

#endif // __kernel_ramdisk_ramdisk_h__
