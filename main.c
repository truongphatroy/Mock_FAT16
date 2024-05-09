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

int main(int argc, char const *argv[])
{
	/* Init Setting */
	// if (FAT_fileSystem_Init("demoWinImage.IMA") != FAT_OK)
	if (FAT_fileSystem_Init("floppy.img") != FAT_OK)
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
