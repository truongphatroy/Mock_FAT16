#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "fat.h"
#include "fat_print.h"
#include "List.h"


typedef struct
{
    uint16_t isFolder;
    uint16_t FirstCluster;
}Fat_Cluster_t;

Fat_Status_t FAT_fileSystem_Init(uint8_t *fileName);
Fat_Status_t FAT_ReadRootDir();
Fat_Status_t FAT_ReadDirEntry(uint16_t FirstCluster);
void FAT_ReadFile(uint16_t FirstCluster);
Fat_Status_t FAT_DisplayConsole();


#endif
