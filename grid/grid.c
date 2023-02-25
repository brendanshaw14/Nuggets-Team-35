/* Grid.c - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See grid.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"

/****************global types**************/
typedef struct grid {
    char* grid;
    hashtable_t* playerTable;
    const int numRows;
    const int numColumns;

} grid_t;



