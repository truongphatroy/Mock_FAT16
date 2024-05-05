#include "console.h"

int main()
{
	if(FAT_fileSystem_Init("floppy.img") != FAT_OK)
	{
		return ;
	}
	FAT_ReadDirectory();
	closeFile();
    return 0;
}
