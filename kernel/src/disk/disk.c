#include "disk.h"
#include "config.h"
#include "io/io.h"
#include "memory/memory.h"
#include "status.h"

static int disk_read_sector(int lba, int total, void *buf) {
  outb(DISK_MASTER_DEVICE_REG, (lba >> 24) | DISK_MASTER_DRIVE_MASK);
  outb(DISK_MASTER_SECTOR_COUNT_REG, total);
  outb(DISK_MASTER_LBA_LO_REG, (unsigned char)(lba & 0xff));
  outb(DISK_MASTER_LBA_MID_REG, (unsigned char)(lba >> 8));
  outb(DISK_MASTER_LBA_HI_REG, (unsigned char)(lba >> 16));
  outb(DISK_MASTER_STATUS_COMMAND_REG, DISK_COMMAND_READ);
  unsigned short *ptr = (unsigned short *)buf;
  for (int i = 0; i < total; ++i) {
    char c = insb(DISK_MASTER_STATUS_COMMAND_REG);
    // TODO: make this an interrupt isntead
    do { // poll until ready to read
      c = insb(DISK_MASTER_STATUS_COMMAND_REG);
    } while (!(c & DISK_STATUS_DRQ_MASK));
    // read 512 bytes
    for (int j = 0; j < 256; ++j) {
      *ptr = insw(DISK_MASTER_IO_REG);
      ++ptr;
    }
  }
  return 0;
}

struct disk disk;

void disk_init() {
  memset(&disk, 0, sizeof(disk));
  disk.type = SAP_OS_DISK_TYPE_REAL;
  disk.sector_size = SAP_OS_DISK_SECTOR_SIZE;
}

struct disk *disk_get(int index) {
  if (index != 0)
    return 0;
  return &disk;
}

int disk_read_block(struct disk *idisk, unsigned int lba, int total,
                    void *buf) {
  if (idisk != &disk)
    return -EIO;
  return disk_read_sector(lba, total, buf);
}