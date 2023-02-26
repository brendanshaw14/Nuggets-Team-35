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
    char* gridString;
    hashtable_t* playerTable;
    int numRows;
    int numColumns;
} grid_t;

//initialize a grid given an input map file
grid_t* grid_init(FILE* inputMap){
    //initialize the new grid structs
    grid_t* grid = mem_malloc(sizeof(grid_t)); //make a new grid
    grid -> playerTable = hashtable_new(26); //make a player table with 26 spots
    //get row and column parameters
    grid -> numRows = file_numLines(inputMap); //set the number of rows
    char* firstLine = file_readLine(inputMap); 
    grid -> numColumns = strlen(firstLine);
    //make the grid string!
    grid -> gridString = mem_malloc((grid -> numColumns + 1) * grid -> numRows + 1); 
    char* nextLine = mem_malloc(grid -> numColumns + 1);
    //put the first line in    
    strcat(firstLine, "\n");
    strcat(grid -> gridString, firstLine);
    //read through the lines of the map, adding the null terminator and pasting them into the gridString
    while ((nextLine = file_readLine(inputMap)) != NULL){
        strcat(nextLine, "\n");
        strcat(grid -> gridString, nextLine);
    }
    return grid;
}

