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

typedef unsigned int SAP_OS_DISK_TYPE;

struct disk {
    SAP_OS_DISK_TYPE type;
    int sector_size;
};

void disk_init();
struct disk* disk_get(int index);
/**
 * @brief Reads one or more blocks from a disk into a buffer.
 *
 * @param idisk Pointer to the disk structure to read from.
 * @param lba Logical Block Addressing (LBA) of the first block to read.
 * @param total Number of blocks to read.
 * @param buf Pointer to the buffer where the read data will be stored.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);
#endif