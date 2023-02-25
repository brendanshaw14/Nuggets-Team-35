/* Grid.c - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See grid.h for detailed info.*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

/****************global types**************/
typedef struct grid {
    char* grid;
    hashtable_t* playerTable;
    int numRows;
    int numColumns;
} grid_t;

//initialize a grid given an input map file
bool grid_init(FILE* inputMap){
    grid_t* grid = mem_malloc(sizeof(grid_t));
    grid -> numRows = file_numLines(inputMap);
    printf("Num rows: %d", grid -> numRows);
    return true;
}

