#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10


#define SAP_OS_TOTAL_INTERRUPTS 512

// HEAP ////////////////////////////////////
// 100MB heap size
#define SAP_OS_HEAP_SIZE_BYTES 104857600
#define SAP_OS_HEAP_BLOCK_SIZE 4096
#define SAP_OS_HEAP_ADDRESS 0x01000000 
#define SAP_OS_HEAP_TABLE_ADDRESS 0x00007E00
/////////////////////////////////////////////

// DISK ////////////////////////////////////
#define SAP_OS_DISK_SECTOR_SIZE 512
#define SAP_OS_DISK_TYPE_REAL 0
///////////////////////////////////////////



#endif