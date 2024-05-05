#include "fat.h"

FILE *pFile = NULL;


Fat_Status_t FAT_readSectors(BootSecotor_t *BootSector, uint32_t IndexSector, uint32_t count, void* bufferOut)
{
    fseek(pFile, IndexSector * BootSector->BytsPerSec, SEEK_SET);
    Fat_Status_t status = (fread(bufferOut, BootSector->BytsPerSec, count, pFile) == count);
    return status;
}

Fat_Status_t FAT_readBootSector(BootSecotor_t *BootSector)
{
	Fat_Status_t status = FAT_OK;
	fseek(pFile, 0x00, SEEK_SET);
    if (fread(BootSector, sizeof(BootSecotor_t), 1, pFile) != 1) 
	{
        status = FAT_ERR;
    }
	return status;
}

Fat_Status_t FAT_CheckBootSignature()
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



//input: current indexCluster
//Output: Next index Cluster
uint16_t FAT_FindNextCluster(BootSecotor_t *BootSector, uint16_t current_fat_entry)
{
    uint8_t tmpFatValue[2]; 	//Save 12 bit of 2byte 
    uint16_t FatValue;		
	//Offset
	uint32_t Offset = (BootSector->BytsPerSec*BootSector->RsvdSecCnt + (current_fat_entry * 3) / 2);
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


//Input : StartDirectorSector, offset in file
//Output: Directory entry
Fat_Status_t FAT_Read_1DirEntry(uint32_t Offset, uint8_t *OutputBuff)
{
	Fat_Status_t status = FAT_OK;
	fseek(pFile, Offset, SEEK_SET);
	status = fread(OutputBuff, 32, 1, pFile);
    return status;
}


Fat_Status_t FAT_ReadFile(BootSecotor_t *BootSector, uint32_t cluster)
{
	Fat_Status_t status = FAT_OK;
	return status;
}

Fat_Status_t openFile(uint8_t *fileName)
{
	Fat_Status_t status = FAT_OK;
    pFile = fopen(fileName, "rb");
    if (pFile == NULL)
    {
        status = FAT_ERR;
    }
    return status;
}

Fat_Status_t closeFile()
{
    fclose(pFile);
    pFile = NULL;
    return FAT_ERR;
}

