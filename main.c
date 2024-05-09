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

#include "console.h"

/*==========================================================================*/
/* MAIN FUNCTON													*/
/*==========================================================================*/

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		printf("Please input file name: main.exe floppy.img\n");
		return 1;
	}
	if (FAT_fileSystem_Init(argv[1]) != FAT_OK)
	{
		return 0;
	}
	/* Display */
	if (FAT_DisplayConsole() != FAT_OK)
	{
		return 0;
	}
	return 0;
}
