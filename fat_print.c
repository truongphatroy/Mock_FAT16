#include "fat_print.h"

/* Show data of BPB */
void Print_Boot(BootSecotor_t BootSector) {
	printf("Bytes per sector:                     %d\n", BootSector.BytsPerSec);
	printf("Sectors per cluster:                  %d\n", BootSector.SecPerClus);
	printf("Number of reserved sectors:           %d\n", BootSector.RsvdSecCnt);
	printf("Number of FATs:                       %d\n", BootSector.NumFATs);
	printf("Max number of root directory entries: %d\n", BootSector.RootEntCnt);
	printf("Total sector count:                   %d\n", BootSector.TotSec16);
	printf("Sectors per FAT:                      %d\n", BootSector.FATSz16);
	printf("\n \n");
}
void Print_Folder_Open()
{
	printf("------------------------------------------------------------------------------------------\n");
	printf("  |Name|\t\t|Type|\t\t|Size|\t\t\t|Time|\t\t|Date|   |\n");
	printf("------------------------------------------------------------------------------------------\n");
}
void Print_One_Entry(RootDirectory_t* entryAddr, uint32_t entryNum)
{
	Read_TimeDate Time;
	Read_TimeDate Date;
	Time.TimeDate = entryAddr->WrtTime;
	Date.TimeDate = entryAddr->WrtDate;
	printf("%d.%s\t\t|", entryNum, entryAddr->Name);
	if(entryAddr->Attr == ATTR_DIRECTORY)
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
