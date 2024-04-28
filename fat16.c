#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fat16.h"

// BPB functions
STATE_t state = STATE_TRUE;
FILE *pFile = NULL;
BPB_t bpb;

uint32_t BPB_FAT_addr(BPB_t *bpb)
{
    return bpb->RsvdSecCnt * bpb->BytsPerSec;
}

uint32_t BPB_Root_addr(BPB_t *bpb)
{
    return BPB_FAT_addr(bpb) + bpb->NumFATs * bpb->FATSz16 * bpb->BytsPerSec;
}

uint32_t BPB_Data_addr(BPB_t *bpb)
{
    return BPB_Root_addr(bpb) + bpb->RootEntCnt * 32;
    // size of root is 32 bytes
}

uint32_t BPB_Data_Sector_count(BPB_t *bpb)
{
    return bpb->TotSec32 - BPB_Data_addr(bpb) / bpb->BytsPerSec;
}

STATE_t read_Bytes(FILE *f, uint32_t offset, void *buffer, uint32_t length)
{
    if (fseek(f, offset, SEEK_SET != 0))
    {
        state = STATE_ERR_READBYTES;
    }

    if (fread(buffer, 1, length, f) != length && ferror(f))
    {
        state = STATE_ERR_SEEK;
    }

    return state;
}

STATE_t read_Boot()
{
    read_Bytes(pFile, 0x00, &bpb, sizeof(bpb));
    print_BPB_Data();
    return STATE_TRUE;
}

/* For test data of BPB */
/* Show data of BPB */
void print_BPB_Data()
{
    uint8_t i = 0;
    printf("FAT addr: 0x%x\n"
           "Root addr: 0x%x\n"
           "Data addr: 0x%x\n",
           BPB_FAT_addr(&bpb),
           BPB_Root_addr(&bpb),
           BPB_Data_addr(&bpb));
    printf("\njmpBoot: ");
    for (i = 0; i < sizeof(bpb.jmpBoot); i++)
    {
        if (i < sizeof(bpb.jmpBoot) - 1)
            printf("%02x:", bpb.jmpBoot[i]);
        else
            printf("%02x \n", bpb.jmpBoot[i]);
    }
    printf("OEMName: %s\n", bpb.OEMName);
    printf("BytsPerSec: %d\n", bpb.BytsPerSec);
    printf("RsvdSecCnt: %d\n", bpb.RsvdSecCnt);
    printf("TotSec32: %d\n", bpb.BytsPerSec);
}

STATE_t openFile(uint8_t *fileName)
{
    pFile = fopen(fileName, "r");
    if (pFile == NULL)
    {
        return STATE_ERR_OPENFILE;
    }
    else
    {
        return STATE_TRUE;
    }
}

void closeFile()
{
    fclose(pFile);
}
