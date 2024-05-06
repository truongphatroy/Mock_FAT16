#ifndef _FAT_PRINT_H_
#define _FAT_PRINT_H_

#include <stdio.h>
#include <stdint.h>

typedef struct
{
    uint8_t FileName[20];		    
    uint8_t Ext[3];
    uint16_t Time;			
    uint16_t Date;			
    uint32_t Size;			
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


void Print_Folder_Open();
void Print_DirectoryEntry(DirectoryEntry_Printf_t *entryAddr, uint32_t entryNum);

#endif
