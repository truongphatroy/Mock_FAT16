#include "fat_print.h"

/* Show data of BPB */
//Input: Data BootSector
void Print_BootSector(BootSec_Print_t *BootSector) 
{
	printf("Bytes per sector:                     %d\n", BootSector->BytsPerSec);
	printf("Sectors per cluster:                  %d\n", BootSector->SecPerClus);
	printf("Number of reserved sectors:           %d\n", BootSector->RsvdSecCnt);
	printf("Number of FATs:                       %d\n", BootSector->NumFATs);
	printf("Sectors per FAT:                      %d\n", BootSector->FATSz16);
	printf("Max number of root directory entries: %d\n", BootSector->RootEntCnt);
	printf("Total sector count:                   %d\n", BootSector->TotSec16);
	printf("\n \n");
}
void Print_Folder_Open()
{
	printf("------------------------------------------------------------------------------------------\n");
	printf("  |Name|\t\t|Type|\t\t|Size|\t\t\t|Time|\t\t|Date|   |\n");
	printf("------------------------------------------------------------------------------------------\n");
}
//print entry
//Input: Data of struct DirectoryEntry_Printf_t
void Print_DirectoryEntry(DirectoryEntry_Printf_t *entryAddr, uint32_t entryNum)
{
	Read_TimeDate Time;
	Read_TimeDate Date;
	Time.TimeDate = entryAddr->WrtTime;
	Date.TimeDate = entryAddr->WrtDate;
	printf("%d.%s\t\t|", entryNum, entryAddr->Name);
	if(entryAddr->isFolder)
	{
		printf("Folder\t\t|\t\t\t");
		printf("%d:%d:%d\t", Time.Time.hour, Time.Time.min, Time.Time.sec*2);
		printf("%d/%d/%d|", Date.Date.day, Date.Date.month, Date.Date.year + 1980u);
	}
	else
	{
		printf("File %s\t|",entryAddr->Extension);
		printf("%d Byte\t\t",entryAddr->FileSize);
		printf("%d:%d:%d\t\t", Time.Time.hour, Time.Time.min, Time.Time.sec*2);
		printf("%d/%d/%d|", Date.Date.day, Date.Date.month, Date.Date.year + 1980u);
	}
	printf("\n");
}

//void GetData_BootSec(void *inputData, BootSec_Print_t *outputData)
//{
//	
//}
//
//void GetData_DirectoryEntry(void *inputData, DirectoryEntry_Printf_t *outputData)
//{
//
//}

