#include <stdint.h>

#include "atapi.h"
#include "io.h"

#define DATA_PORT 0x1F0
#define ERROR_PORT 0x1F1
#define FEATURES_PORT 0x1F1
#define SECTOR_COUNT_PORT 0x1F2
#define LBA_LOW_PORT 0x1F3
#define LBA_MID_PORT 0x1F4
#define LBA_HIGH_PORT 0x1F5
#define DRIVE_SELECT_PORT 0x1F6
#define COMMAND_PORT 0x1F7
#define STATUS_PORT 0x1F7

#define ATAPI_COMMAND_PACKET 0xA0
#define SCSI_COMMAND_READ10 0x28

extern "C"
{

  bool atapi_read_sector(uint32_t lba, uint16_t *buffer)
  {
    // 1. Select the drive
    outb(DRIVE_SELECT_PORT, 0x40); // Assuming primary master

    // 2. Send the PACKET command
    outb(COMMAND_PORT, ATAPI_COMMAND_PACKET);

    // Wait until the drive is ready
    while (inb(STATUS_PORT) & 0x80)
      ; // BSY bit

    // 3. Send the SCSI READ(10) command
    uint8_t packet[12] = {0};
    packet[0] = SCSI_COMMAND_READ10;
    packet[2] = (lba >> 24) & 0xFF;
    packet[3] = (lba >> 16) & 0xFF;
    packet[4] = (lba >> 8) & 0xFF;
    packet[5] = lba & 0xFF;
    packet[7] = 0; // We're reading just one sector
    packet[8] = 1;

    for (int i = 0; i < 6; i++)
    {
      outw(DATA_PORT, *(uint16_t *)(packet + i * 2));
    }

    // 4. Read the data
    for (int i = 0; i < 256; i++)
    { // 512 bytes/sector, 2 bytes/word = 256 words
      buffer[i] = inw(DATA_PORT);
    }

    // 5. Check for errors
    if (inb(ERROR_PORT))
    {
      return false; // Error occurred
    }

    return true;
  }

} // extern "C"
