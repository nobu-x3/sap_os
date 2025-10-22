#ifndef DISK_H
#define DISK_H

// clang-format off
#define DISK_MASTER_IO_REG              0x1F0
#define DISK_MASTER_DEVICE_REG          0x1F6
#define DISK_MASTER_SECTOR_COUNT_REG    0x1F2
#define DISK_MASTER_LBA_LO_REG          0x1F3
#define DISK_MASTER_LBA_MID_REG         0x1F4
#define DISK_MASTER_LBA_HI_REG          0x1F5
#define DISK_MASTER_STATUS_COMMAND_REG  0x1F7

#define DISK_SLAVE_IO_REG               0x170
#define DISK_SLAVE_DEVICE_REG           0x176
#define DISK_SLAVE_SECTOR_COUNT_REG     0x172
#define DISK_SLAVE_LBA_LO_REG           0x173
#define DISK_SLAVE_LBA_MID_REG          0x174
#define DISK_SLAVE_LBA_HI_REG           0x175
#define DISK_SLAVE_STATUS_COMMAND_REG   0x177

#define DISK_COMMAND_READ               0x20

#define DISK_MASTER_DRIVE_MASK          0xE0
#define DISK_STATUS_DRQ_MASK           0x08
// clang-format on

int disk_read_sector(int lba, int total_sectors, void *buf);

#endif