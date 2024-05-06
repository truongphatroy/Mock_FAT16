#include "console.h"
#include <string.h>

BootSecotor_t BootSector;

LinkedList ListSaveDirectory;

uint16_t FileCount;

Fat_Cluster_t Files[50] = {0};

/*
	StartFAT = BootSector.RsvdSecCnt;
	
    StartRootDir = BootSector.RsvdSecCnt + BootSector.NumFATs*BootSector.FATSz16;
    
    StartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs*BootSector.FATSz16 + 
	(32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
*/

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
	return FAT_OK;
}


//Get datto from fat.c, fat.h to print
void GetDataToprint(RootDirectory_t Input,DirectoryEntry_Printf_t *Output)
{
	strcpy(Output->FileName, Input.Name);
	strcpy(Output->Ext, Input.Extension);
	Output->Date = Input.WrtDate;
	Output->Time = Input.WrtTime;
	Output->Size = Input.FileSize;
	Output->isFolder = (Input.Attr == ATTR_DIRECTORY);
}


//Input: Offset start Sector 
//output: List file or folder 
Fat_Status_t FAT_ReadRootDir()
{
	uint32_t AddrStartRootDir = (BootSector.RsvdSecCnt + BootSector.NumFATs*BootSector.FATSz16)*BootSector.BytsPerSec;
	uint16_t i;
	FileCount = 0;
	for(i = 0; i < BootSector.RootEntCnt; i++)
	{
		uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AddrStartRootDir);
		if(DirEntryBuff == NULL)
		{
			return FAT_ERR;
		}
		RootDirectory_t DirecEntry = *(RootDirectory_t*)DirEntryBuff;
		DirectoryEntry_Printf_t DirecEntryPrint;
		if(DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0]  == 0x00)
		{
			return FAT_OK;
		}
		if(DirecEntry.Attr == 0x0F || DirecEntry.Name[0] == 0x2E)
		{
			AddrStartRootDir+= 32;
			continue;
		}
		
		GetDataToprint(DirecEntry , &DirecEntryPrint);
		
		Files[FileCount].isFolder= DirecEntryPrint.isFolder;
		Files[FileCount].FirstCluster = DirecEntry.FstClusLO;
		FileCount++;
		
		Print_DirectoryEntry(&DirecEntryPrint,FileCount);
		AddrStartRootDir+= 32;
		free(DirEntryBuff);
	}
	return FAT_OK;
}


//Input: First cluster 
//output: List file or folder 
Fat_Status_t FAT_ReadDirEntry(uint16_t FirstCluster)
{
	uint32_t currentCluster = FirstCluster;
	
	uint32_t SecStartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16  
		+ (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	FileCount = 0;
	do
	{		
		//Address Start of FirstCluster
		uint32_t AdrrCurrentCluster = (SecStartData_Area + (currentCluster - 2)*BootSector.SecPerClus)*BootSector.BytsPerSec;
		
		uint16_t i;
		for(i = 0; i < BootSector.BytsPerSec *BootSector.SecPerClus ; i++)
		{
			uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AdrrCurrentCluster );
			DirectoryEntry_Printf_t DirecEntryPrint;
			RootDirectory_t DirecEntry = *(RootDirectory_t*)DirEntryBuff;
			if(DirEntryBuff == NULL)
			{
				return FAT_ERR;
			}
			
			if(DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0] == 0x00)
			{
				return FAT_OK;
			}
			if(DirecEntry.Attr == 0x0F)
			{
				AdrrCurrentCluster+= 32;
				continue;
			}
			
			GetDataToprint(DirecEntry , &DirecEntryPrint);
			
			Files[FileCount].isFolder= DirecEntryPrint.isFolder;
			Files[FileCount].FirstCluster = DirecEntry.FstClusLO;
			FileCount++;
			Print_DirectoryEntry(&DirecEntryPrint,FileCount);
			
			AdrrCurrentCluster+= 32;
			free(DirEntryBuff);
		}
		currentCluster = FAT_FindNextCluster(&BootSector, currentCluster);
	}while(currentCluster < 0xFF8);
	return FAT_OK;
}

//Input: First cluster 
//output: Data of file....
void FAT_ReadFile(uint16_t FirstCluster)
{
	uint32_t currentCluster = FirstCluster;
	uint32_t SecStartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16  
		+ (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	do
	{		
		//Address Start of FirstCluster
		uint32_t AdrrFirstCluster = (SecStartData_Area + (currentCluster - 2)*BootSector.SecPerClus)*BootSector.BytsPerSec;
		ShitfOffset(AdrrFirstCluster);
		uint16_t i;
		for(i = 0; i < BootSector.BytsPerSec *BootSector.SecPerClus ; i++)
		{
			uint8_t Buff = Fgetc();
			if(Buff == 0x00)
			{
				continue;
			}
			printf("%c",Buff);
		}
		currentCluster = FAT_FindNextCluster(&BootSector, currentCluster);
	}while(currentCluster < 0xFF8);
}

Fat_Status_t FAT_DisplayConsole()
{
	Fat_Status_t status = FAT_OK;
	Print_Folder_Open();
	status = FAT_ReadRootDir();
	ListAddHead(&ListSaveDirectory, 1);
	while(1)
	{
		int32_t userChoice = -1;
		do
		{
			printf("\n 0.Exit  ");
			printf("\n-1.Back  ");
			printf("\n Select : ");
			scanf("%d" ,&userChoice);
			fflush(stdin);
		}while(userChoice < -1);
		if(userChoice == 0)
		{
			system("cls");
			printf("Bai Bai :((<3))");
			ListDeleteAll(&ListSaveDirectory);
			return closeFile();;
		}
		else if(userChoice > FileCount)
		{
			printf("\nSelect again: ");
		}
		else if(userChoice == -1)
		{
			ListDeleteTail(&ListSaveDirectory);
			if(ListSaveDirectory.head == NULL)
			{
				system("cls");
				printf("Bai Bai :((<3))");
				status = closeFile();
				return status;
			}
			if(ListSaveDirectory.head->data == 1)
			{
				system("cls");
				status = FAT_ReadRootDir();
				ListDeleteTail(&ListSaveDirectory);
			}
			else
			{
				system("cls");
				status = FAT_ReadDirEntry(ListSaveDirectory.tail->data);
				ListDeleteTail(&ListSaveDirectory);
			}
		}
		else
		{
			if(Files[userChoice - 1].isFolder)
			{
				ListAddHead(&ListSaveDirectory, Files[userChoice-1].FirstCluster);
				system("cls");
				Print_Folder_Open();
				status = FAT_ReadDirEntry(Files[userChoice-1].FirstCluster);
			}
			else
			{
				system("cls");
				FAT_ReadFile(Files[userChoice-1].FirstCluster);
			}
		}
	}
	return status;
}


