/* Grid.h - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

#ifndef _GRID_H_
#define _GRID_H_

//includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../player/player.h"


/****************global types**************/
// typedef struct grid grid_t;  // opaque to users of the module

typedef struct player player_t; 

typedef struct grid {
    char* gridString;
    player_t* playerArray[27]; 
    counters_t* goldTable;
    int numRows;
    int numColumns;
    int numPlayers;
    int goldRemaining;
} grid_t;

/***********grid_init*************/
/* Initializes a new grid given an input map file
    -sets all players in the grid array to NULL
    -sets the numPlayers to 0, goldRemaining to 250
    -construct the grid gridString
Requires: a pointer to a valid input map file. 
Returns: NULL if file isn't accessible; Otherwise returns
a new grid. 
Notes: We assume the map, if not NULL, is a valid map. 
*/
grid_t* grid_init(FILE* inputMap);


/**********grid_placeGold*********/
/*
*/
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int goldTotal, int seed);

/**********grid_addPlayer*********/
/* Adds the given player into the map into a random location
returns true if added,
false upon failure
*/
bool grid_addPlayer(grid_t* grid, player_t* newPlayer);


/**********grid_delete*********/
/*Deletes the given grid
*/
void grid_delete(grid_t* grid);
#endif