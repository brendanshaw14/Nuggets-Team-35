/* Grid.c - the grid module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See grid.h for detailed info.*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../player/player.h"

/****************file-local global variables*/
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

static bool addPlayerInArray(grid_t* grid, player_t* player); 

//initialize a grid given an input map file
grid_t* grid_init(FILE* inputMap){
    //return null if map doesn't exist
    if (inputMap == NULL){
        return NULL;
    }
    //initialize the new grid structs
    grid_t* grid = mem_malloc(sizeof(grid_t)); //make a new grid
    grid->goldTable = counters_new();
    //set variables to their defaults
    grid -> numPlayers = 0;
    grid -> goldRemaining = 250;
    // initialize the player to be NULL
    for (int i = 0; i < MaxPlayers + 1; i++) {
        grid->playerArray[i] = NULL; 
    }
    //get row and column parameters
    grid -> numRows = file_numLines(inputMap); //set the number of rows
    char* firstLine = file_readLine(inputMap); 
    grid -> numColumns = strlen(firstLine);
    //make the grid string and initialize empty!
    grid -> gridString = mem_malloc((grid -> numColumns + 2) * grid -> numRows + 1); 
    grid -> gridString[0] = '\0';
    char* nextLine; 
    //put the first line in    
    strcat(grid -> gridString, firstLine);
    strcat(grid -> gridString, "\n");
    //read through the lines of the map, adding the null terminator and pasting them into the gridString
    while ((nextLine = file_readLine(inputMap)) != NULL){
        strcat(grid -> gridString, nextLine);
        strcat(grid -> gridString, "\n");
        mem_free(nextLine);
    }
    mem_free(firstLine);
    mem_free(nextLine);
    return grid;
}
/*	
create a counter for the gold 
find the min and max pieces per pile
while not all the pieces have been added
    add gold to the counter
    add the gold to the map 
    increment
when the last piece is reached before 250, add the remaining gold to that pile
*/
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int GoldTotal, int seed){
    int goldPlaced = 0;
    int pilesPlaced = 0;
    int minPerPile = (GoldTotal/maxPiles);
    int maxPerPile = (GoldTotal/minPiles);
    //loop through random indexes
    int index;
    char currentChar;
    int goldInPile;
    //set the seed
    if (seed != -1){
        srand(seed);
    }
    else{
        srand(getpid());
    }
    //add an approximate amount to numPiles piles
    while (goldPlaced <= GoldTotal - maxPerPile){
        index = rand() % (grid -> numColumns * grid -> numRows) + 1; //get a random index in the map
        currentChar = grid -> gridString[index]; //get the character at that index
        //if that character is valid (a room char)
        if (currentChar == '.'){
            goldInPile = rand() % (maxPerPile - minPerPile) + minPerPile; //amt of gold in this pile
            counters_set(grid -> goldTable, index, goldInPile);
            goldPlaced += goldInPile;
            pilesPlaced ++;
            grid -> gridString[index] = '*';
        }
    }
    //place the final pile
    index = rand() % (grid -> numColumns * grid -> numRows) + 1; //get a random index in the map
    goldInPile = GoldTotal - goldPlaced;
    counters_set(grid -> goldTable, index, goldInPile);
    goldPlaced += goldInPile;
    pilesPlaced ++;
    return true;
}

//adds the given player struct to the given grid's playerArray
bool grid_addPlayer(grid_t* grid, player_t* newPlayer){
    //check if grid or player is null
    if (grid == NULL || newPlayer == NULL){
        return false;
    }
    //get a random positon
    int index;
    char currentChar = ' ';
    //loop until a room character is found
    while (currentChar != '.'){
        index = rand() % (grid -> numColumns * grid -> numRows) + 1; //get a random index in the map
        currentChar = grid -> gridString[index];
        //add the character when found 
        if (currentChar == '.'){
            newPlayer -> player_position = index;
            // add this player into the playerArray
            if (!addPlayerInArray(grid, newPlayer)){
                return false; 
            }
        }
    }
    return true;
}

void grid_delete(grid_t* grid){
    //free the players
    for (int i = 0; i < MaxPlayers+1; i ++){
        player_delete(grid -> playerArray[i], grid);
    }
    //free the counters
    counters_delete(grid->goldTable);
    //free the grid string 
    mem_free(grid->gridString);
    //free the grid
    mem_free(grid);
    return;
}

static bool addPlayerInArray(grid_t* grid, player_t* player) {
    int i = 0; 
    for (i = 0; i < MaxPlayers + 1; i++) {
        if (grid->playerArray[i] == NULL) 
            break; 
    } 
    if (i == MaxPlayers + 1) {
        // current array is full
        fprintf(stderr, "Error: Cannot add new player in the array.\n");
        return false; 
    }
    // add this player
    grid->playerArray[i] = player; 
    return true; 
}