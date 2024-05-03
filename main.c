#include "fat.h"
extern FILE *disk;
extern Start_Area_t St;
extern Fat_Cluster_t Files[100];
extern uint32_t FileCount;

int main()
{
    if(openFile("floppy.img") != FAT_OK)
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
			printf("\nSelect :_");
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
			printf("\nSelect again:_");
		}
		else
		{
			if(Files[userChoice - 1].isFolder)
			{
				system("cls");
				readDirectory(disk,Files[userChoice - 1].FirstCluster + St.StartData_Area - 2);
				printf("b. <=== Back\n");
			}
			else
			{
				FAT_Read_File(disk, Files[userChoice - 1].FirstCluster);
			}
		}
	}
    closeFile();
    return 0;
}
