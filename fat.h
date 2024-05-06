#ifndef _FAT_H_
#define _FAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BOOT_SIGNATURE_OFFSET		0x1FE
#define ATTR_READ_ONLY 				0x01
#define	ATTR_HIDDEN 				0x02
#define ATTR_SYSTEM 				0x04
#define ATTR_VOLUME_ID 				0x08
#define ATTR_DIRECTORY    			0x010
#define ATTR_ARCHIVE 				0x20
#define	ATTR_LONG_FILE_NAME 		0x0F

#pragma pack(1)
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
} BootSecotor_t;

typedef struct
{
    uint8_t Name[8];		         //0x00
    uint8_t Extension[3];
    uint8_t Attr;				    //0x0B
    uint8_t NTRes;					//0x0C
    uint8_t CrtTimeTenth;			//0x0D
    uint16_t CrtTime;				//0x0E

    uint16_t CrtDate;				//0x10
    uint16_t LstAccDate;			//0x12
    uint16_t FstClusHI;				//0x14
    uint16_t WrtTime;				//0x16
    uint16_t WrtDate;				//0x18
    uint16_t FstClusLO;				//0x1A
    uint32_t FileSize;				//0x1C
} RootDirectory_t;
#pragma pack()

typedef enum
{
    FAT_ERR = 0,
    FAT_OK = 1,
}Fat_Status_t;


Fat_Status_t openFile(uint8_t *fileName);
Fat_Status_t closeFile();

#endif
