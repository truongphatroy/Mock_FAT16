#ifndef FAT16_H
#define FAT16_H

/* FAT16 file system header */

#include <stdint.h>

#pragma pack(1)
typedef struct
{
    uint8_t jmpBoot[3];
    unsigned char OEMName[8];
    uint16_t BytsPerSec;
    uint8_t SecPerClus;
    uint16_t RsvdSecCnt;

    uint8_t NumFATs;
    uint16_t RootEntCnt;
    uint16_t TotSec16;
    uint8_t Media;
    uint16_t FATSz16; // sectors per FAT
    uint16_t SecPerTrk;
    uint16_t NumHeads;
    uint32_t HiddSec;

    uint32_t TotSec32; // total size
} BPB_t;

typedef struct
{
    unsigned char Name[11];
    uint8_t Attr;
    uint8_t NTRes;
    uint8_t CrtTimeTenth;
    uint16_t CrtTime;

    uint16_t CrtDate;
    uint16_t LstAccDate;
    uint16_t FstClusHI;
    uint16_t WrtTime;
    uint16_t WrtDate;
    uint16_t FstClusLO;
    uint32_t FileSize;
} DIR_t;
#pragma pack()

typedef enum
{
    STATE_TRUE = 1,
    STATE_ERR_SEEK,
    STATE_ERR_READBYTES,
    STATE_ERR_OPENFILE,
} STATE_t;

STATE_t openFile(uint8_t *fileName);
void closeFile();
STATE_t read_Boot();
void print_BPB_Data();
STATE_t read_Root_dir();
void print_cluster(FILE *f, BPB_t *bpb, DIR_t *dir);
#endif