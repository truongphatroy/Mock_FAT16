#ifndef _FAT_PRINT_H_
#define _FAT_PRINT_H_

#include <stdio.h>
#include <stdint.h>


typedef struct
{
    uint8_t jmpBoot[3];			//0x00
    uint8_t OEMName[8];			//0x03
    uint16_t BytsPerSec;		//0x0B
    uint8_t SecPerClus;			//0x0D
    uint16_t RsvdSecCnt;		//0x0E     

    uint8_t NumFATs;			//0x10     
    uint16_t RootEntCnt;		//0x11     
    uint16_t TotSec16;			//0x13     
    uint8_t Media;				//0x15     
    uint16_t FATSz16; 			// 0x16      sectors per FAT
    uint16_t SecPerTrk;			//0x18     
    uint16_t NumHeads;			//0x1A     
    uint32_t HiddSec;			//0x1C     
    uint32_t TotSec32; 			//0x20 total size
} BootSec_Print_t;

typedef struct
{
    uint8_t Name[20];		         //0x00
    uint8_t Extension[3];
    uint16_t WrtTime;				//0x16
    uint16_t WrtDate;				//0x18
    uint16_t FstClusLO;				//0x1A
    uint32_t FileSize;				//0x1C
    uint8_t isFolder;
}DirectoryEntry_Printf_t;

//========Convert time in 16 bit
typedef struct
{
	uint16_t day :5;
	uint16_t month : 4;
	uint16_t year : 7;
}Date16_t;

typedef struct
{
	uint16_t sec : 5;
	uint16_t hour : 6;
	uint16_t min : 5;
}Time16_t;
//-------------------

typedef union
{
	uint16_t TimeDate;		
	Date16_t Date;
	Time16_t Time;
}Read_TimeDate;
/* Show data of BPB */
void Print_BootSector(BootSec_Print_t *BootSector);
void Print_Folder_Open();
void Print_DirectoryEntry(DirectoryEntry_Printf_t *entryAddr, uint32_t entryNum);

#endif
