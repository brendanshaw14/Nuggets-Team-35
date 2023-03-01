/* server.c file for the Nuggets game project
Authors: Brendan Shaw, Ming Cheng, Romeo Myrthil
CS50- Winter 2023
*/

//Includes: 
#include "../support/message.h"
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../grid/grid.h"
#include "../player/player.h"

/****************file-local global variables**************/
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles


//function headers:

grid_t* initializeGame(const int argc, const char* argv[]);
bool handleTimeout(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);


/********************Main******************/
/* Psuedocode:
*/
int main(const int argc, const char* argv[]){
    
    grid_t* grid = initializeGame(argc, argv);
    //initialize the message stream
    message_init(stderr);
    //loop through messages
    message_loop(grid, 100, handleTimeout, handleInput, handleMessage);
    //end the message stream
    message_done();

    return 0;
}

/*************initializeGame*************/
/*  Check that either 1 or 2 arguments were provided
    if not, print error message, exit non-zero
    if so, verify the map can be opened for reading
        if 1 argument
            srand(getpid());
            load the map
        else 
            srand(argv[3]);
NOTE: THIS METHOD ACCEPTS EXTRANEOUS CHARACTERS INCLUDED IN THE SEED ARGUMENT SO LONG AS AN INT CAN BE PARSED
*/

grid_t* initializeGame(const int argc, const char* argv[]){
    //hold the seed and map
    int seed = -1;
    //if one or two args were provided
    if (argc == 2 || argc == 3){
        //test the map is openeable for reading
        FILE* map;
        map = fopen(argv[1], "r");
        //if not opened, print error and exit non-zero
        if (map == NULL){
            fprintf(stderr, "Error: Unable to read map file input.");
            exit(2);
        }
        
        if (argc == 3){
            if (sscanf(argv[2], "%d", &seed) != 1){
                fprintf(stderr, "Error: Unable to read seed");
                exit(3);
            }
        }
        grid_t* grid = grid_init(map);
        grid_placeGold(grid, GoldMinNumPiles, GoldMaxNumPiles, GoldTotal, seed);
        return grid;
    }
    else{ 
        fprintf(stderr, "Error: Incorrect number of arguments");
        exit(1);
    }
    return NULL;
}

//handle timeout starter function
bool handleTimeout(void* arg){
    return true;
}

//handle input starter function
bool handleInput(void* arg){
    return true;
}

//handle message starter function
bool handleMessage(void* arg, const addr_t from, const char* message){
    printf("%s\n", message);
    return true;
}

