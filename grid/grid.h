/* Grid.h - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

//includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"


/****************global types**************/
typedef struct grid grid_t;  // opaque to users of the module

/****************global types**************/
typedef struct grid {
    char* gridString;
    hashtable_t* playerTable;
    counters_t* goldTable;
    int numRows;
    int numColumns;
    int numPlayers;
    int goldRemaining;
} grid_t;

/***********grid_init*************/
/* Initializes a new grid given an input map file
save the number of rows and columns
Loop through each line of the map
*/
grid_t* grid_init(FILE* inputMap);


/**********grid_placeGold*********/
/* Places the gold in piles with random amounts of pieces randomly across the map. 
Takes a grid to place into, a max number of piles, and a min number of piles. 
Returns True upon success 
*/
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int seed);