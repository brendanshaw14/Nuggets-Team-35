/* Grid.h - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

//includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"

/***********grid_init*************/
/* Initializes a new grid given an input map file
save the number of rows and columns
Loop through each line of the map
*/
bool grid_init(FILE* inputMap);