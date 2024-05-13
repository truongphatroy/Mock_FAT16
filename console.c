/*---------------------------------------------------------------------------
 *
 *	Embedded Developer - Group 3
 *	MOCK PROJECT - Mock Project - FAT Filetra
 *	Date: 2024/08
 *
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/* Including File													*/
/*==========================================================================*/
#include "console.h"
#include <string.h>

/*==========================================================================*/
/* Declare Global Variable													*/
/*==========================================================================*/

BootSecotor_t BootSector;

static LinkedList ListSaveDirectory;

static uint16_t FileCount;

static Fat_Cluster_t FileEntry;

/*==========================================================================*/
/* Declare Global Variable													*/
/*==========================================================================*/

/* Init */
Fat_Status_t FAT_fileSystem_Init(uint8_t *fileName)
{
	ListInitialize(&ListSaveDirectory); // Initialize the directory linked list
	if (openFile(fileName) != FAT_OK)
	{
		return closeFile(); // Close the file and return error status
	}
	if (FAT_CheckBootSignature() != FAT_OK)
	{
		return closeFile();
	}
	if (FAT_readBootSector(&BootSector) != FAT_OK)
	{
		return closeFile();
	}
	if (FAT_CheckFAT_Type(&BootSector) == FAT_TYPE_ERR)
	{
		printf("! NOT FAT12 \n");
		return closeFile();
	}
	return FAT_OK; // Return success status
}

// Get datto from fat.c, fat.h to print
void GetDataToprint(RootDirectory_t Input, DirectoryEntry_Printf_t *Output)
{
	// strcpy(Output->FileName, Input.Name);
	memcpy(Output->FileName, Input.Name, sizeof(Input.Name));
	strcpy(Output->Ext, Input.Extension);
	Output->Date = Input.WrtDate;
	Output->Time = Input.WrtTime;
	Output->Size = Input.FileSize;
	Output->isFolder = (Input.Attr == ATTR_DIRECTORY);
}

// Input: Offset start Sector
// output: List file or folder
Fat_Status_t FAT_ReadRootDir()
{
	uint32_t AddrStartRootDir = (BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16) * BootSector.BytsPerSec;
	uint16_t i;
	FileCount = 0;
	for (i = 0; i < BootSector.RootEntCnt; i++)
	{
		// Read one directory entry
		uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AddrStartRootDir);
		if (DirEntryBuff == NULL)
		{
			return FAT_ERR;
		}
		RootDirectory_t DirecEntry = *(RootDirectory_t *)DirEntryBuff;
		DirectoryEntry_Printf_t DirecEntryPrint;
		// Check for end of directory
		if (DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0] == 0x00)
		{
			return FAT_OK;
		}
		// Skip if dir name[0] is "." or dir is long name type
		if (DirecEntry.Attr == 0x0F || DirecEntry.Name[0] == 0x2E)
		{
			AddrStartRootDir += 32;
			continue;
		}

		GetDataToprint(DirecEntry, &DirecEntryPrint);

		FileCount++;

		Print_DirectoryEntry(&DirecEntryPrint, FileCount);
		AddrStartRootDir += 32; // Move to the next entry
		free(DirEntryBuff);
	}
	return FAT_OK;
}

/* new root for root*/
Fat_Status_t FAT_ReadCheckRootDirEntry(int32_t fileNo)
{
	uint32_t AddrStartRootDir = (BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16) * BootSector.BytsPerSec;
	uint16_t i;
	FileCount = 0;
	for (i = 0; i < BootSector.RootEntCnt; i++)
	{
		uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AddrStartRootDir);
		if (DirEntryBuff == NULL)
		{
			return FAT_ERR;
		}
		RootDirectory_t DirecEntry = *(RootDirectory_t *)DirEntryBuff;
		DirectoryEntry_Printf_t DirecEntryPrint;
		if (DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0] == 0x00)
		{
			return FAT_OK;
		}
		// Skip if dir name[0] is "." or dir is long name type
		if (DirecEntry.Attr == 0x0F || DirecEntry.Name[0] == 0x2E)
		{
			AddrStartRootDir += 32;
			continue;
		}

		GetDataToprint(DirecEntry, &DirecEntryPrint);
		FileCount++;

		if (FileCount == fileNo)
		{
			// FileEntry.FirstCluster = DirecEntryPrint.isFolder;
			FileEntry.isFolder = DirecEntryPrint.isFolder;
			FileEntry.FirstCluster = DirecEntry.FstClusLO;
			break;
		}
		AddrStartRootDir += 32;
		free(DirEntryBuff);
	}
	return FAT_OK;
}
/*
 * new function for check the enterred key entry is the folder or file
 */

Fat_Status_t FAT_ReadCheckDirEntry(uint32_t FirstCluster, int32_t fileNo)
{
	uint32_t currentCluster = FirstCluster;

	uint32_t SecStartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16 + (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	FileCount = 0;
	do
	{
		// Address Start of FirstCluster
		uint32_t AdrrCurrentCluster = (SecStartData_Area + (currentCluster - 2) * BootSector.SecPerClus) * BootSector.BytsPerSec;
		uint16_t i;
		for (i = 0; i < BootSector.BytsPerSec * BootSector.SecPerClus; i++)
		{
			uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AdrrCurrentCluster);
			DirectoryEntry_Printf_t DirecEntryPrint;
			RootDirectory_t DirecEntry = *(RootDirectory_t *)DirEntryBuff;
			if (DirEntryBuff == NULL)
			{
				return FAT_ERR;
			}
			// check entry is deleted or end of entries
			if (DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0] == 0x00)
			{
				return FAT_OK;
			}
			// check "." and long file name entry
			if (DirecEntry.Attr == 0x0F || DirecEntry.Name[0] == 0x2E)
			{
				AdrrCurrentCluster += 32;
				continue;
			}

			GetDataToprint(DirecEntry, &DirecEntryPrint); // Prepare directory entry data for printing
			FileCount++;

			/* new code*/
			// If the current file count matches the requested file number
			if (FileCount == fileNo)
			{
				FileEntry.isFolder = DirecEntryPrint.isFolder;
				FileEntry.FirstCluster = DirecEntry.FstClusLO;
				break;
			}

			/* end new code*/

			AdrrCurrentCluster += 32;
			free(DirEntryBuff);
		}
		currentCluster = FAT_FindNextCluster(&BootSector, currentCluster);
	} while (currentCluster < 0xFF8 || FileCount != fileNo);
	return FAT_OK;
}

/*end of above function*/
// Input: First cluster
// output: List file or folder
Fat_Status_t FAT_ReadDirEntry(uint16_t FirstCluster)
{
	uint32_t currentCluster = FirstCluster;

	uint32_t SecStartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16 + (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	FileCount = 0;
	do
	{
		// Address Start of FirstCluster
		uint32_t AdrrCurrentCluster = (SecStartData_Area + (currentCluster - 2) * BootSector.SecPerClus) * BootSector.BytsPerSec;
		uint16_t i;
		for (i = 0; i < BootSector.BytsPerSec * BootSector.SecPerClus; i++)
		{
			uint8_t *DirEntryBuff = FAT_Read_1DirEntry(AdrrCurrentCluster);
			DirectoryEntry_Printf_t DirecEntryPrint;
			RootDirectory_t DirecEntry = *(RootDirectory_t *)DirEntryBuff;
			if (DirEntryBuff == NULL)
			{
				return FAT_ERR;
			}
			if (DirecEntry.Name[0] == 0xE5 || DirecEntry.Name[0] == 0x00)
			{
				return FAT_OK;
			}
			if (DirecEntry.Attr == 0x0F || DirecEntry.Name[0] == 0x2E)
			{
				AdrrCurrentCluster += 32;
				continue;
			}

			GetDataToprint(DirecEntry, &DirecEntryPrint);

			FileCount++;
			Print_DirectoryEntry(&DirecEntryPrint, FileCount);

			AdrrCurrentCluster += 32;
			free(DirEntryBuff);
		}
		currentCluster = FAT_FindNextCluster(&BootSector, currentCluster);
	} while (currentCluster < 0xFF8);
	return FAT_OK;
}

// delete buffer
void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

// Input: First cluster
// output: Data of file....
void FAT_ReadFile(uint16_t FirstCluster)
{
	uint32_t currentCluster = FirstCluster;
	uint32_t SecStartData_Area = BootSector.RsvdSecCnt + BootSector.NumFATs * BootSector.FATSz16 + (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
	do
	{
		// Address Start of FirstCluster
		uint32_t AdrrFirstCluster = (SecStartData_Area + (currentCluster - 2) * BootSector.SecPerClus) * BootSector.BytsPerSec;
		ShitfOffset(AdrrFirstCluster);
		uint16_t i;
		for (i = 0; i < BootSector.BytsPerSec * BootSector.SecPerClus; i++)
		{
			uint8_t Buff = Fgetc();
			if (Buff == 0x00) // Check for end of file
			{
				continue; // Continue to the next byte
			}
			printf("%c", Buff);
		}
		currentCluster = FAT_FindNextCluster(&BootSector, currentCluster);
	} while (currentCluster < 0xFF8); // Continue until the end of the cluster chain
}

Fat_Status_t FAT_DisplayConsole()
{
	Fat_Status_t status = FAT_OK;
	Print_Folder_Open();
	status = FAT_ReadRootDir();
	ListAddTail(&ListSaveDirectory, 1); // Add root directory to the save directory list
	// ListPrint(&ListSaveDirectory);		// Print the list
	while (1)
	{
		int32_t userChoice = -1;
		do
		{
			printf("\n 0.Exit  ");
			printf("\n-1.Back  ");
			printf("\n Select : ");
			scanf("%d", &userChoice);
			fflush(stdin);
		} while (userChoice < -1);

		switch (userChoice)
		{
		case 0:
			system("cls"); // Clear the console
			// printf("Bai Bai :((<3))");
			ListDeleteAll(&ListSaveDirectory);
			return closeFile();
		case -1: // back
			if (ListSaveDirectory.tail->data == 1)
			{
				continue;
			}
			ListDeleteTail(&ListSaveDirectory);
			if (ListSaveDirectory.tail->data == 1)
			{
				system("cls");
				Print_Folder_Open();
				status = FAT_ReadRootDir();
			}
			else
			{
				system("cls");
				Print_Folder_Open();
				status = FAT_ReadDirEntry(ListSaveDirectory.tail->data);
			}
			break;
		default: // go ahead
			if (userChoice > FileCount)
			{
				printf("\nSelect again: ");
			}
			else
			{
				if (ListSaveDirectory.tail->data == 1)
				{
					system("cls");
					status = FAT_ReadCheckRootDirEntry(userChoice);
				}
				else
				{
					system("cls");
					status = FAT_ReadCheckDirEntry(ListSaveDirectory.tail->data, userChoice);
				}
				// status = FAT_ReadCheckDirEntry(ListSaveDirectory.tail->data, userChoice);
				if (FileEntry.isFolder)
				{
					system("cls");
					Print_Folder_Open();
					status = FAT_ReadDirEntry(FileEntry.FirstCluster);
					// add to list
					ListAddTail(&ListSaveDirectory, FileEntry.FirstCluster);
					// print node of list
					// ListPrint(&ListSaveDirectory);
				}
				else
				{
					system("cls");
					FileCount = 0;
					FAT_ReadFile(FileEntry.FirstCluster);
					ListAddTail(&ListSaveDirectory, FileEntry.FirstCluster);
					// ListPrint(&ListSaveDirectory);
				}
			}
			break;
		}
	}
	return status;
}
