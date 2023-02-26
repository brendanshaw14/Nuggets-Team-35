/* Grid.c - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See grid.h for detailed info.*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

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

/****************file-local global variables*/
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

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
//	initialize hashtable
	//loop while goldplaced != goldTotal
      //get a random index in grid string
	  //check if index is valid position
		//get a random int newGoldPile between goldMin and goldMax
		//if goldplaced + newGoldPile < goldTotal
		  //insert in hashtable
bool grid_placeGold(grid_t* grid, int maxPile, int minPile){
    hashtable_t* goldTable = hashtable_init();
    return true;
}