#include "fat.h"
#include "fat_print.h"

FILE *disk = NULL;

BootSecotor_t BootSector;
Start_Area_t St;	//Start area

//File Check;
uint32_t FileCount = 0;
Fat_Cluster_t Files[100] = {0};

Fat_Status_t readSectors(FILE* pFile, uint32_t IndexSector, uint32_t count, void* bufferOut)
{
    fseek(pFile, IndexSector * BootSector.BytsPerSec, SEEK_SET);
    Fat_Status_t status = (fread(bufferOut, BootSector.BytsPerSec, count, pFile) == count);
    return status;
}

Fat_Status_t readBootSector(FILE* pFile)
{
	Fat_Status_t status = FAT_OK;
	fseek(pFile, 0x00, SEEK_SET);
    status = fread(&BootSector, sizeof(BootSector), 1, pFile) > 0;
    St.StartFAT = BootSector.RsvdSecCnt;
    St.StartRootDir = St.StartFAT + BootSector.NumFATs*BootSector.FATSz16;
    St.StartData_Area = St.StartRootDir + (32 * BootSector.RootEntCnt + BootSector.BytsPerSec - 1) / BootSector.BytsPerSec;
    return status;
}

Fat_Status_t CheckBootSignature(FILE* pFile)
{
	Fat_Status_t status = FAT_OK;
	uint16_t BootSignature;
	fseek(pFile, 510, SEEK_SET);
    status = (fread(&BootSignature,sizeof(BootSignature) , 1, pFile) == 1);
    if(BootSignature != 0xAA55)
    {
    	status = FAT_ERR;
    }
	return status;
}


Fat_Status_t readDirectory(FILE* pFile,uint16_t StartDirectorSector)
{
	Fat_Status_t status = FAT_OK;
	FileCount = 0;
	uint16_t indexStart =  StartDirectorSector;
	while(1)
	{
		RootDirectory_t *RootDirectoryBuff = (RootDirectory_t*) malloc(BootSector.BytsPerSec);
	    status = readSectors(pFile, indexStart, 1, RootDirectoryBuff);
	    
	    uint32_t idx;
	    for (idx = 0; idx < BootSector.BytsPerSec/32  ; idx++)
	    {
	    	if(RootDirectoryBuff[idx].Name[0] == 0xE5 || RootDirectoryBuff[idx].Name[0] == 0x00)
	    	{
	    		free(RootDirectoryBuff);
	    		return FAT_OK;
			}
	        if(RootDirectoryBuff[idx].Attr == ATTR_LONG_FILE_NAME)
	        {
	            continue;
	        }
	        Files[FileCount].FirstCluster = RootDirectoryBuff[idx].FstClusLO;
	        Files[FileCount].isFolder = (RootDirectoryBuff[idx].Attr == ATTR_DIRECTORY);
	        printf("Files[%d].FirstCluster: %d\n",FileCount+1,Files[FileCount].FirstCluster );
	        FileCount++;
	        Print_One_Entry(&RootDirectoryBuff[idx],FileCount);
	    }
	    free(RootDirectoryBuff);
	    indexStart = indexStart+1 ;
	}
	
    return status;
}

//input: current indexCluster
//Output: Next index Cluster
uint16_t FAT_Find_Next_Cluster(FILE *pFile, uint16_t current_fat_entry)
{
    uint8_t tmpFatValue[2]; 	//Save 12 bit of 2byte 
    uint16_t FatValue;		
	//Offset
	uint32_t Offset = (BootSector.BytsPerSec*BootSector.RsvdSecCnt + (current_fat_entry * 3) / 2);
	fseek(pFile, Offset,0);
	//Read 2 byte in current offset
    tmpFatValue[0] = fgetc(pFile);
    tmpFatValue[1] = fgetc(pFile);


    if (current_fat_entry % 2 == 0)
	{
        FatValue = (tmpFatValue[0] | ((uint16_t)(tmpFatValue[1] & 0xF) << 8));
    }
    else
	{
        FatValue = ((tmpFatValue[1] << 4) | ((uint16_t)tmpFatValue[0] >> 4));
    }
    return FatValue;
}

void FAT_Read_File(FILE *pFile, uint32_t Firtclusterindex)
{
	uint32_t index = Firtclusterindex;
	uint32_t sectorindex = St.StartData_Area + index - 2;
	do
	{
		printf("\n==================%d\n",index );
		uint8_t *ClusterBuff = (uint8_t*)malloc(BootSector.BytsPerSec * BootSector.SecPerClus);	    //Allocation 1 cluster	
		readSectors(pFile, sectorindex,BootSector.SecPerClus,ClusterBuff);
		printf("%s ", ClusterBuff);		
		free(ClusterBuff);		
		index = FAT_Find_Next_Cluster(pFile,index);
		sectorindex = St.StartData_Area + index - 2;
		
	}while(index < 0xFF8);
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

void closeFile()
{
    fclose(disk);
    disk = NULL;
}

