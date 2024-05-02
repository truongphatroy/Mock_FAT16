#include "fat.h"
#include "fat_print.h"

FILE *disk = NULL;

BootSecotor_t BootSector;
uint8_t *fatBuff = NULL;
RootDirectory_t *RootDirectoryBuff = NULL;
uint32_t g_RootDirectoryEnd;
Start_Area_t St;

//File Check;
uint32_t FileCount = 0;
Fat_Cluster_t Files[100];

Fat_Status_t readSectors(FILE* pFile, uint32_t IndexSector, uint32_t count, void* bufferOut)
{
    fseek(pFile, IndexSector * BootSector.BytsPerSec, SEEK_SET);
    Fat_Status_t status = (fread(bufferOut, BootSector.BytsPerSec, count, pFile) == count);
    return status;
}

Fat_Status_t readBootSector(FILE* pFile)
{
	Fat_Status_t status = FAT_OK;
    status = fread(&BootSector, sizeof(BootSector), 1, pFile) > 0;
    St.StartFAT = BootSector.RsvdSecCnt;
    St.StartRootDir = St.StartFAT + BootSector.NumFATs*BootSector.FATSz16;
    St.StartData_Area = St.StartRootDir + (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
//    printf("%d %d %d\n",St.StartFAT,St.StartRootDir,St.StartData_Area );
    return status;
}

Fat_Status_t readFat(FILE* pFile)
{
    fatBuff = (uint8_t*) malloc(BootSector.FATSz16 * BootSector.BytsPerSec);
    return readSectors(pFile, BootSector.RsvdSecCnt, BootSector.FATSz16, fatBuff);
}

Fat_Status_t readDirectory(FILE* pFile,uint16_t StartDirectorSector)
{
	Fat_Status_t status = FAT_OK;
	FileCount = 0;
	uint16_t indexStart =  StartDirectorSector;
	Print_Folder_Open();
	while(1)
	{
		RootDirectoryBuff = (RootDirectory_t*) malloc(BootSector.BytsPerSec);
	    status = readSectors(pFile, indexStart, 1, RootDirectoryBuff);
	    
	    uint32_t idx = 0;
	    for (idx = 0; idx < BootSector.BytsPerSec/32  ; idx++)
	    {
	    	if(RootDirectoryBuff[idx].Name[0] == 0xE5 || RootDirectoryBuff[idx].Name[0] == 0x00)
	    	{
	    		free(RootDirectoryBuff);
	    		return FAT_OK;
			}
	        if(RootDirectoryBuff[idx].Attr == ATTR_LONG_FILE_NAME || RootDirectoryBuff[idx].Name[0] == 0x2E )
	        {
	            continue;
	        }
	        Files[FileCount].FirstCluster = RootDirectoryBuff[idx].FstClusLO;
	        Files[FileCount].isFolder = (RootDirectoryBuff[idx].Attr == ATTR_DIRECTORY);
//	        printf("Files[%d].FirstCluster: %d\n",FileCount,Files[FileCount].FirstCluster );
	        FileCount++;
	        Print_One_Entry(&RootDirectoryBuff[idx],FileCount);
	    }
	    free(RootDirectoryBuff);
	}
	indexStart++;
    return status;
}


uint16_t FAT_Find_Next_Cluster(uint32_t n)// pass first cluster index
{

	uint16_t nextAddress;
	if ((n % 2) == 0)
	{
		nextAddress = (((fatBuff[((3 * n)/2) + 1] << 8) | (fatBuff[(3 * n)/2]))& 0x0FFFu);
	}
	else
	{
		nextAddress = (((fatBuff[(3 * n)/2] >> 4)  | (fatBuff[((3 * n)/2) + 1] << 4 ))& 0x0FFFu); 	
	}
	return nextAddress;
}
void FAT_Read_File(FILE *pFile, uint32_t Firtclusterindex)
{
	uint32_t index = Firtclusterindex;
	uint32_t sectorindex = g_RootDirectoryEnd + index - 2;
	do
	{
		uint8_t *SectorBuff = (uint8_t*)malloc(BootSector.BytsPerSec * BootSector.SecPerClus);	    //Allocation 1 cluster	
		readSectors(pFile, sectorindex,BootSector.SecPerClus,SectorBuff);
		printf("%s", SectorBuff);		
		free(SectorBuff);		
		index = FAT_Find_Next_Cluster(index);
		if(0xFF8u <= index)
		{
			break;
		}
		sectorindex = g_RootDirectoryEnd + index - 2;
	}while(1);
	printf("\n");
}

Fat_Status_t openFile(uint8_t *fileName)
{
	Fat_Status_t status = FAT_OK;
    disk = fopen(fileName, "rb");
    if (disk == NULL)
    {
        status = FAT_ERR;
    }
    return status;
}
void closeFAT()
{
	free(fatBuff);
}

void closeFile()
{
    fclose(disk);
    disk = NULL;
}

