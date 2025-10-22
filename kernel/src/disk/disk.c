#include "disk.h"
#include "io/io.h"

int disk_read_sector(int lba, int total, void *buf) {
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