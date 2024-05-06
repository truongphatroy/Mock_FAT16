#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "fat.h"
#include "fat_print.h"
#include "List.h"


typedef struct
{
    uint16_t isFolder;
    uint16_t FirstCluster;
}Fat_Cluster_t;


#endif
