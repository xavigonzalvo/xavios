#include <stdint.h>
#include <stddef.h>

#include "list_files.h"
#include "printf.h"
#include "sprintf.h"
#include "terminal.h"
#include "atapi.h"

// Volume Descriptor Type
#define BOOT_RECORD 0
#define PRIMARY_VOLUME_DESCRIPTOR 1
#define SUPPLEMENTARY_VOLUME_DESCRIPTOR 2
#define VOLUME_PARTITION_DESCRIPTOR 3
#define VOLUME_DESCRIPTOR_SET_TERMINATOR 255

// Directory Record Flags
#define DIRECTORY 2

// Volume Descriptor header
struct VolumeDescriptorHeader
{
  uint8_t type;
  char identifier[5]; // Should be "CD001"
  uint8_t version;
} __attribute__((packed));

// Primary Volume Descriptor (PVD)
struct PVD
{
  VolumeDescriptorHeader header;
  uint8_t unused1[1];
  char system_id[32];
  char volume_id[32];
  uint8_t unused2[8];
  uint32_t volume_space_size[2];
  uint8_t unused3[32];
  uint16_t volume_set_size[2];
  uint16_t volume_sequence_number[2];
  uint16_t logical_block_size[2];
  uint32_t path_table_size[2];
  uint32_t type_l_path_table;
  uint32_t opt_type_l_path_table;
  uint32_t type_m_path_table;
  uint32_t opt_type_m_path_table;
  uint8_t root_directory_record[34]; // This is the root directory descriptor
                                     // ... Other members follow, but for simplicity, we're not defining them.
} __attribute__((packed));

// Directory Entry structure
struct DirectoryRecord
{
  uint8_t length;
  uint8_t ext_attr_length;
  uint32_t extent_location[2];
  uint32_t data_length[2];
  uint8_t recording_date_time[7];
  uint8_t file_flags;
  uint8_t file_unit_size;
  uint8_t interleave_gap_size;
  uint16_t volume_sequence_number[2];
  uint8_t file_id_length;
  char file_id[]; // Variable length
} __attribute__((packed));

int strncmp(const char *str1, const char *str2, size_t n)
{
  while (n--)
  {
    if (*str1 != *str2)
    {
      return *(unsigned char *)str1 - *(unsigned char *)str2;
    }
    if (!*str1)
    { // If end of string is reached
      return 0;
    }
    str1++;
    str2++;
  }
  return 0;
}

extern "C"
{

  void list_root_files()
  {
    uint16_t buffer[2048 / sizeof(uint16_t)]; // 2 KB buffer for reading CD sectors

    char bb[100];

    int sector = 0;
    for (sector = 0; sector < 30; sector++)
    {
      if (!atapi_read_sector(sector, buffer))
      {
        simple_printf("Failed to read sector");
        return;
      }

      PVD *potentialPVD = (PVD *)buffer;
      if (strncmp(potentialPVD->header.identifier, "CD001", 5) == 0 && potentialPVD->header.type == PRIMARY_VOLUME_DESCRIPTOR)
      {
        break; // We found the PVD
      }
      sector++;

      // simple_sprintf(bb, "sector: %d (%s)\n", sector, potentialPVD->header.identifier);
      terminal_writestring(bb);
    }

    // Read the Primary Volume Descriptor
    if (!atapi_read_sector(sector, buffer))
    {
      simple_printf("Failed to read PVD\n");
      return;
    }

    PVD *pvd = (PVD *)buffer;

    // Get the root directory record from the PVD
    DirectoryRecord *rootRecord = (DirectoryRecord *)pvd->root_directory_record;

    // Calculate the number of sectors the root directory occupies
    int rootDirSectors = (rootRecord->data_length[0] + 2047) / 2048; // Assuming 2048-byte sectors

    // simple_printf("Root directory occupies %d sectors\n", rootDirSectors);

    for (int i = 0; i < 10; i++)
    {
      // simple_sprintf(bb, "%d ", ((uint8_t *)buffer)[i]);
    }
    terminal_writestring("\n");

    // simple_sprintf(bb, "%s %d %d\n",
    //                pvd->header.identifier,
    //                pvd->header.type, pvd->header.version);
    terminal_writestring(bb);

    for (int i = 0; i < rootDirSectors; i++)
    {
      // simple_sprintf(bb, "%d\n", i);
      terminal_writestring(bb);

      // if (!atapi_read_sector(rootRecord->extent_location[0] + i, buffer))
      // {
      //   simple_printf("Failed to read root directory data\n");
      //   return;
      // }

      // uint8_t *dirData = (uint8_t *)buffer;
      // while (dirData[0] != 0)
      // {
      //   DirectoryRecord *dirRecord = (DirectoryRecord *)dirData;

      //   // if (!(dirRecord->file_flags & DIRECTORY))
      //   // {
      //   for (int j = 0; j < dirRecord->file_id_length; j++)
      //   {
      //     simple_printf("%c", dirRecord->file_id[j]);
      //   }
      //   simple_printf("\n");
      //   // }

      //   dirData += dirData[0];
      // }
    }
  }

} // extern "C"
