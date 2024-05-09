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
#include "fat.h"

/*==========================================================================*/
/* Declare Global Variable													*/
/*==========================================================================*/
FILE *pFile = NULL;

/* Read one sector from Bootselector */
// This function reads the boot sector of a FAT file system.
Fat_Status_t FAT_readBootSector(BootSecotor_t *BootSector)
{
    Fat_Status_t status = FAT_OK;
    // Move the file pointer to the beginning of the file
    fseek(pFile, 0x00, SEEK_SET);
      // Read the boot sector into the provided BootSector structure
    if (fread(BootSector, sizeof(BootSecotor_t), 1, pFile) != 1)
    {
        status = FAT_ERR;
    }
    return status;
}
// Check boot signature
// This function checks the boot sector signature of a FAT file system.
Fat_Status_t FAT_CheckBootSignature(BootSecotor_t *BootSector)
{
    Fat_Status_t status = FAT_OK;
    uint16_t BootSignature;
    // Move the file pointer to the 510th byte
    fseek(pFile, 510, SEEK_SET);
    status = (fread(&BootSignature, sizeof(BootSignature), 1, pFile) == 1);
    // Check if the boot signature is correct
	if (BootSignature != 0xAA55)
    {
        status = FAT_ERR;
    }
    return status;
}


/*
	FatStartSector = BPB_ResvdSecCnt;
	FatSectors = BPB_FATSz * BPB_NumFATs;
	
	
	RootDirStartSector = FatStartSector + FatSectors;
	RootDirSectors = (32 * BPB_RootEntCnt + BPB_BytsPerSec – 1) / BPB_BytsPerSec;
	
	DataStartSector = RootDirStartSector + RootDirSectors;
	DataSectors = BPB_TotSec – DataStartSector;	
	
*/
// This function checks the type of the FAT file system.
Fat_Status_t FAT_CheckFAT_Type(BootSecotor_t *BootSector)
{
    Fat_Status_t status = FAT_OK;
    uint32_t SecStartData_Area = BootSector->RsvdSecCnt + BootSector->NumFATs * BootSector->FATSz16 + 
	(32 * BootSector->RootEntCnt + BootSector->BytsPerSec - 1) / BootSector->BytsPerSec;
	
	uint32_t DataSectors = BootSector->TotSec16 - SecStartData_Area;
    uint32_t CountofClusters = DataSectors/ BootSector->SecPerClus;
 // Check if the count of clusters exceeds the maximum allowed value
	if(CountofClusters >= 4086)
    {
    	status = FAT_TYPE_ERR;
	}
    return status;
}
// input: current indexCluster
// Output: Next index Cluster
uint16_t FAT_FindNextCluster(BootSecotor_t *BootSector, uint16_t current_fat_entry)
{
    uint8_t tmpFatValue[2]; // Save 12 bit of 2byte
    uint16_t FatValue;
   // Calculate the offset in the FAT
    uint32_t Offset = (BootSector->BytsPerSec * BootSector->RsvdSecCnt + (current_fat_entry * 3) / 2);
     // Move the file pointer to the calculated offset
	fseek(pFile, Offset, 0);
    // Read 2 byte in current offset
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

// Input : StartDirectorSector, offset in file
// Output: Directory entry
uint8_t *FAT_Read_1DirEntry(uint32_t Offset)
{
    fseek(pFile, Offset, SEEK_SET);
    // Allocate memory for the directory entry buffer
	uint8_t *OutputBuff = (uint8_t *)malloc(32); // free it after call
    if (OutputBuff == NULL)
    {
        return NULL;
    }
    fread(OutputBuff, 1, 32, pFile);
    return OutputBuff;
}
// This function shifts the file pointer to the specified offset.
void ShitfOffset(uint32_t Offset)
{
    fseek(pFile, Offset, SEEK_SET);
}
// This function reads a character from the file.
uint8_t Fgetc()
{
    return fgetc(pFile);
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
//Close file
Fat_Status_t closeFile()
{
    fclose(pFile);
    pFile = NULL;
    return 0;
}
