#include "console.h"
#include <string.h>

Start_Area_t St;

BootSecotor_t BootSector;

LinkedList ListSaveDirectory;

//File Check;
uint32_t FileCount = 0;
Fat_Cluster_t Files[20] = {0};

Fat_Status_t FAT_fileSystem_Init(uint8_t *fileName)
{
	ListInitialize(&ListSaveDirectory);
	if(openFile(fileName) != FAT_OK)
    {
        return closeFile();
	}
	if(FAT_CheckBootSignature() != FAT_OK)
	{
		return closeFile();
	}
    if(FAT_readBootSector(&BootSector) != FAT_OK)
    {
        return closeFile();
    }
    St.StartFAT = BootSector.RsvdSecCnt;
    St.StartRootDir = St.StartFAT + BootSector.NumFATs*BootSector.FATSz16;
    St.StartData_Area = St.StartRootDir + 
	(32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	return FAT_OK;
}

Fat_Status_t FAT_ReadDirectory()
{
	Fat_Status_t status = FAT_OK;
	uint16_t i, OffsetEntry = St.StartRootDir*BootSector.BytsPerSec;
	FileCount = 0;
	for(i = 0; i < BootSector.RootEntCnt; i++)
	{
		uint8_t *EntryBuff = (uint8_t *)malloc(32);
		status = FAT_Read_1DirEntry(OffsetEntry, &EntryBuff);
	
		if(DirEntry->Name[0] == 0xE5 || DirEntry->Name[0] == 0x00)
		{
			free(EntryBuff);
			return status;
		}
		if(DirEntry->Attr == ATTR_LONG_FILE_NAME)
		{
			continue;
		}
		DirectoryEntry_Printf_t EntryPrint;
		strcpy(EntryPrint.Name,DirEntry->Name );
		strcpy(EntryPrint.Extension,DirEntry->Extension );
		EntryPrint.WrtTime = DirEntry->WrtTime;
		EntryPrint.WrtDate = DirEntry->WrtDate;
		EntryPrint.FstClusLO = DirEntry->FstClusLO;
		EntryPrint.FileSize = DirEntry->FileSize;
		
		FileCount++;
		Print_DirectoryEntry(&EntryPrint, FileCount);
		free(EntryBuff);
		OffsetEntry+=32;
	}
}





