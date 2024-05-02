#include "fat.h"
extern FILE *disk;
extern Start_Area_t St;
extern Fat_Cluster_t Files[100];
int main()
{
    if(openFile("floppy.img") != FAT_OK)
    {
    	closeFile();
        return 0;
	}
    if(readBootSector(disk) != FAT_OK)
    {
        closeFile();
        return 0;
    }
    if(readFat(disk) != FAT_OK )
    {
    	closeFAT();
        closeFile();
        return 0;
    }
    if(readDirectory(disk,St.StartRootDir) != FAT_OK)
    {
        closeFile();
        return 0;
    }
    
    
    closeFAT();
    closeFile();
    return 0;
}
