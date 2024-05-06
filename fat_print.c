#include "fat_print.h"


void Print_Folder_Open()
{
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("  |Name|\t\t|Type|\t\t|Size|\t\t\t|Time|\t\t|Date|   \t |\n");
	printf("--------------------------------------------------------------------------------------------------\n");
}
//print entry
//Input: Data of struct DirectoryEntry_Printf_t
void Print_DirectoryEntry(DirectoryEntry_Printf_t *entryAddr, uint32_t entryNum)
{
	Read_TimeDate Time;
	Read_TimeDate Date;
	Time.TimeDate = entryAddr->Time;
	Date.TimeDate = entryAddr->Date;
	printf("%d.%s\t\t|", entryNum, entryAddr->FileName);
	if(entryAddr->isFolder)
	{
		printf("Folder\t\t|\t\t\t\t");
		printf("%d:%d:%d\t", Time.Time.hour, Time.Time.min, Time.Time.sec*2);
		printf("%d/%d/%d|", Date.Date.day, Date.Date.month, Date.Date.year + 1980u);
	}
	else
	{
		printf("File %s\t|",entryAddr->Ext);
		printf("%d Byte\t\t\t",entryAddr->Size);
		printf("%d:%d:%d\t\t", Time.Time.hour, Time.Time.min, Time.Time.sec*2);
		printf("%d/%d/%d|", Date.Date.day, Date.Date.month, Date.Date.year + 1980u);
	}
	printf("\n");
}


