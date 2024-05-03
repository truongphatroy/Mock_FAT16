#include "fat.h"
extern FILE *disk;
extern Start_Area_t St;
extern Fat_Cluster_t Files[100];
extern uint32_t FileCount;

int main()
{
    if(openFile("demoWinImage.IMA") != FAT_OK)
    {
    	closeFile();
        return 0;
	}
	if(CheckBootSignature(disk) != FAT_OK)
	{
		closeFile();
        return 0;
	}
    if(readBootSector(disk) != FAT_OK)
    {
        closeFile();
        return 0;
    }
    printf("|--Root Directory\n");
    Print_Folder_Open();
    if(readDirectory(disk,St.StartRootDir) != FAT_OK)
    {
        closeFile();
        return 0;
    }
    printf("0. Exit file\n");

	while(1)
	{
		int userChoice = -1;
		do
		{
			printf("\nSelect : ");
			scanf("%d" ,&userChoice);
			fflush(stdin);
		}while(userChoice <= -1);
		if(userChoice == 0)
		{
			closeFile();
			return 0;
		}
		else if(userChoice > FileCount)
		{
			printf("\nSelect again: ");
		}
		else
		{
			if(Files[userChoice - 1].isFolder)
			{
				system("cls");
				Print_Folder_Open();
				readDirectory(disk,Files[userChoice - 1].FirstCluster + St.StartData_Area - 2);
				if(Files[userChoice - 1].FirstCluster == 0)
				{
					readDirectory(disk,St.StartRootDir);
				}
				printf("1. <=== Reset\n");
				printf("2. <=== Back\n");
			}
			else
			{
				system("cls");
				FAT_Read_File(disk, Files[userChoice - 1].FirstCluster);
				printf("2. <=== Back\n");
			}
		}
	}
    closeFile();
    return 0;
}
