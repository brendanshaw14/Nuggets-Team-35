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

/****************global types**************/
typedef struct grid {
    char* gridString;
    player_t* playerArray[27]; 
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
/*	
create a counter for the gold 
find the min and max pieces per pile
while not all the pieces have been added
    add gold to the counter
    add the gold to the map 
    increment
when the last piece is reached before 250, add the remaining gold to that pile
*/
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int seed){
    counters_t* goldCounter = counters_new(); //make a counters to store the gold piles and their indexes
    int goldPlaced = 0;
    int pilesPlaced = 0;
    int minPerPile = (GoldTotal/maxPiles);
    int maxPerPile = (GoldTotal/minPiles);
    printf("min per pile: %d", minPerPile);
    printf("max per pile: %d", maxPerPile);
    //loop through random indexes
    int index;
    char currentChar;
    int goldInPile;
    //set the seed
    if (seed != 0){
        srand(seed);
        printf("got the seed");
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
            counters_set(goldCounter, index, goldInPile);
            goldPlaced += goldInPile;
            printf("\nAdded %d gold to the pile", goldInPile);
            pilesPlaced ++;
            grid -> gridString[index] = '*';
        }
    }
    //place the final pile
    index = rand() % (grid -> numColumns * grid -> numRows) + 1; //get a random index in the map
    goldInPile = GoldTotal - goldPlaced;
    counters_set(goldCounter, index, goldInPile);
    goldPlaced += goldInPile;
    pilesPlaced ++;
    printf("Number of piles: %d, Total gold: %d", pilesPlaced, goldPlaced);
    return true;
}


bool grid_addPlayer(grid_t* grid, player_t* newPlayer){
    //check if grid or player is null
    if (grid == NULL || newPlayer == NULL){
        return false;
    }
    //get a random positon
    int index;
    char* currentChar = ' ';
    while (currentChar != '.'){
        index = rand() % (grid -> numColumns * grid -> numRows) + 1; //get a random index in the map
        currentChar = grid -> gridString[index];
        if (currentChar == '.'){
            newPlayer -> player_position = index;
        }
    }
    return true;
}