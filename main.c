#include "console.h"

int main()
{
	if(FAT_fileSystem_Init("demoWinImage.IMA") != FAT_OK)
	{
		return 0;
	}
	if(FAT_DisplayConsole() != FAT_OK)
	{
		return 0;
	}
    return 0;
}
