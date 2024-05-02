#ifndef _FAT_PRINT_H_
#define _FAT_PRINT_H_
#include "fat.h"

/* Show data of BPB */
void Print_Boot(BootSecotor_t BootSector);
void Print_Folder_Open();
void Print_One_Entry(RootDirectory_t* entryAddr, uint32_t entryNum);

#endif