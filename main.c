#include <stdio.h>
#include "fat16.h"

uint8_t fileName[] = "floppy.img";
int main()
{
    STATE_t state = STATE_TRUE;

    state = openFile(fileName);
    read_Boot();

    /* For testing */
    switch (state)
    {
    case STATE_ERR_OPENFILE:
        printf("Error opening file!\n");
        break;

    case STATE_TRUE:
        printf("OK all\n");
        break;

    default:
        printf("default");
        break;
    }

    return 0;
}