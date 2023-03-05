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
#include "../libcs50/mem.h"

/****************file-local global variables**************/
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles
static const int PLAYER_RADIUS = 5;    // player visibility radius
static const int TIME_TIL_TIMEOUT = 100;  // seconds until the server timesout
const int SPEC_IDX = 26; // the index of spectator. 26 because the length of the array is 26 + 1. array counts from slot 0-26


//function headers:

grid_t* initializeGame(const int argc, const char* argv[]);
bool handleTimeout(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);
static char assignLetter(int arrayIdx);



/********************Main******************/
/* Psuedocode:
*/
int main(const int argc, const char* argv[]){
    
    grid_t* grid = initializeGame(argc, argv);
    //initialize the message stream
    message_init(stderr);
    //loop through messages
    message_loop(grid, TIME_TIL_TIMEOUT, handleTimeout, handleInput, handleMessage);
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
            fprintf(stderr, "Error: Unable to read map file input.\n");
            exit(2);
        }
        
        if (argc == 3){
            if (sscanf(argv[2], "%d", &seed) != 1){
                fprintf(stderr, "Error: Unable to read seed\n");
                exit(3);
            }
        }
        grid_t* grid = grid_init(map);
        grid_placeGold(grid, GoldMinNumPiles, GoldMaxNumPiles, GoldTotal, seed);
        return grid;
    }
    else{ 
        fprintf(stderr, "Error: Incorrect number of arguments\n");
        exit(1);
    }
    return NULL;
}

//handle timeout starter function
bool handleTimeout(void* arg){
    grid_t* gameGrid = arg;

    for (int i = 0; i < MaxPlayers+1; i++) {

      if (gameGrid->playerArray[i] != NULL){
        char quitMessage[50];  // no message should have to be over lentgh 100. we can change this
        sprintf(quitMessage, "QUIT Server timed out: No action in %d seconds", TIME_TIL_TIMEOUT);
        message_send(gameGrid->playerArray[i]->player_address, (const char*)quitMessage);
      }
    }
    grid_delete(gameGrid);
    
    return true;
}

//handle input starter function
bool handleInput(void* arg){
    return true;
}

//handle message starter function
bool handleMessage(void* arg, const addr_t from, const char* message){

  // printf("Hello handleMessage!\n");

    // passing args into a grid
  grid_t* gameGrid = arg;

    // if message is PLAY
  if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) 
  {
    const char* content = message + strlen("PLAY ");
    

      //    if numPlayers == maxPlayers
    if (gameGrid->numPlayers == MaxPlayers) {
        //      message_send to client saying server is full
      message_send(from, "QUIT Game is full: no more players can join.");
        //    return false to continue loop
      return false;  // i believe returning false will continue the loop. not sure
    }

      // add a check if players name is empty then send the error message
      // QUIT Sorry - you must provide player's name.
    if (strlen(content) > MaxNameLength) {
      message_send(from, "QUIT Name to long: max length is 50");

      return false;
    }

    char playerLetter = assignLetter(gameGrid->numPlayers);

      // initializing player if there was room for more players
    player_t* newPlayer = player_init(gameGrid, from, content, false, PLAYER_RADIUS, playerLetter);  // 5 for radius. not set.

    grid_addPlayer(gameGrid, newPlayer);
      // i believe i still have to increment num players.  REMOVE THIS COMMENT
    gameGrid->numPlayers++;
    
      // message_send OK letter, where letter is the assigned letter
    char letterMessage[100];  // no message should have to be over lentgh 100. we can change this
    sprintf(letterMessage, "OK %c", playerLetter);
    message_send(from, (const char*)letterMessage);
      // message_send(from, "OK dude");
      // sending the info about the grid
    char rowsAndColumnsMessage[100];
    sprintf(rowsAndColumnsMessage, "GRID %d %d", gameGrid->numRows, gameGrid->numColumns);
    message_send(from, (const char*)rowsAndColumnsMessage);

      // updatinf visibility and sending it to client
    player_updateVisibility(newPlayer, gameGrid);
    
    // char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
    char* displayMessage = malloc(strlen(newPlayer->player_seen) + 10);
    displayMessage[strlen(newPlayer->player_seen) + 9] = '\0';

    sprintf(displayMessage, "DISPLAY\n%s", newPlayer->player_seen);
    message_send(from, displayMessage);

    free(displayMessage);
  } 

    //  if message is SPECTATE
  if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0)
  {
    const char* content = message + strlen("SPECTATE"); 

    // c boolean for whether there is a spectator already
    int specExisted = 0;

    // checking if there is a player at the spectator index
    if (gameGrid->playerArray[SPEC_IDX] != NULL) {
      player_t* currSpectator = gameGrid->playerArray[SPEC_IDX];
        // making sure it is a spectator
      if (currSpectator->player_isSpectator) {
        message_send(currSpectator->player_address, "QUIT You have been replaced by a new spectator.");
        player_delete(currSpectator, gameGrid);
          // inserting a new spectator at that index
        player_t* newSpec = player_init(gameGrid, from, content, true, 0, ' ');  // no letter for spectator. will not be used
        gameGrid->playerArray[SPEC_IDX] = newSpec;
        specExisted = 1;
      }
    }
    

    if (specExisted == 0) {
      player_t* newSpec = player_init(gameGrid, from, content, true, 0, ' ');  // no letter for spectator. will not be used
      gameGrid->playerArray[SPEC_IDX] = newSpec;
    } // do i need to do more in here?

    // show the grid 
    char rowsAndColumnsMessage[100];
    sprintf(rowsAndColumnsMessage, "GRID %d %d", gameGrid->numRows, gameGrid->numColumns);
    message_send(from, (const char*)rowsAndColumnsMessage);

    // sending the visibility to spec
    player_updateSpecVisibility(gameGrid->playerArray[SPEC_IDX], gameGrid);
    // char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
    char* displayMessage = malloc(strlen(gameGrid->playerArray[SPEC_IDX]->player_seen) + 10);
    displayMessage[strlen(gameGrid->playerArray[SPEC_IDX]->player_seen) + 9] = '\0';
    sprintf(displayMessage, "DISPLAY\n%s", gameGrid->playerArray[SPEC_IDX]->player_seen);

    message_send(from, (const char*)displayMessage);
    free(displayMessage);
  }
    //  if message is KEY
  if (strncmp(message, "KEY ", strlen("KEY ")) == 0)
  {
    
    counters_print(gameGrid->goldTable, stdout);
    

    const char* content = message + strlen("KEY ");

    // printf("\n\n\n\n\n\n\ncontent should be here!!!!!!!!!!!! %s\n\n\n\n\n\n", content);

      // finding the player the move is attempted on
    player_t* movingPlayer;                                 // change this variable name
    int playerIdx;
    for (int i = 0; i < (MaxPlayers + 1); i++) {
      if (gameGrid->playerArray[i] == NULL) {
        continue;
        // jump to the next iteration of the loop as 
        // nothing can be done with a NULL player
      }
      if (message_eqAddr(gameGrid->playerArray[i]->player_address, from)) {
        movingPlayer = gameGrid->playerArray[i];
        playerIdx = i;
        break;
      }
    }

    // movingPlayer->player_isSpectator is uninitialised for some reason
    bool isSpectator = movingPlayer->player_isSpectator;

    // printf("\n\n\n\n\n\n\ncontent should be here!!!!!!!!!!!! %s\n\n\n\n\n\n", content);

    if (isSpectator) {

      printf("\n\n%s\n\n", content);

      if (strcmp(content, "Q") == 0) {
          // quitting the spectator
        message_send(from, "QUIT Thanks for watching!");
        gameGrid->playerArray[playerIdx] = NULL;
        player_delete(movingPlayer, gameGrid);

        return false; // continue message loop
      } else {
        printf("\n\n\n\n\nshould print something\n\n\n\n\n");
        message_send(from, "ERROR Invalid keystroke: Spectators can't move");
        return false;
      }
    }
    
      // check if player tries to quit
    // printf("%s", content);
    if (strcmp(content, "Q") == 0) {
      // printf("%s", content); 
      // printf("Hello inside quit thing");
      message_send(movingPlayer->player_address, "QUIT Thanks for playing!");
        // gameGrid->playerArray[playerIdx] = NULL;  // dont need to do this i believe

      // player_delete(movingPlayer, gameGrid);

      // setting player to inactive to be able to keep its stats
      movingPlayer->player_isActivate = false;

      return false;  // continue the message loop
    }


    int goldBeforeMove = movingPlayer->player_amountOfGold;

      // player move handles if player picked up gold
    if (!player_move(movingPlayer, gameGrid, content[0])) {
        // sending an error message to the client that attempted a move
      message_send(from, "ERROR Invalid keystroke");
      return false;
    } else {
      if (goldBeforeMove == movingPlayer->player_amountOfGold) {

        for (int i = 0; i < (MaxPlayers + 1); i++) {
          // skipping the loop for spots in the array that are not players yet
          if (gameGrid->playerArray[i] == NULL) {
            continue;  
          }

          if (gameGrid->playerArray[i]->player_isSpectator) {
            
            char goldMessage[100];
            sprintf(goldMessage, "GOLD %d %d %d", 0, 0, gameGrid->goldRemaining);
            message_send(gameGrid->playerArray[i]->player_address, (const char*)goldMessage);
            player_updateSpecVisibility(gameGrid->playerArray[i], gameGrid);
            
            char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
            sprintf(displayMessage, "DISPLAY\n%s", gameGrid->playerArray[i]->player_seen);
            message_send(gameGrid->playerArray[i]->player_address, (const char*)displayMessage);



          // only sending a message if the player is still active
          } else if (gameGrid->playerArray[i]->player_isActivate){

            char goldMessage[100];
            sprintf(goldMessage, "GOLD %d %d %d", 0, gameGrid->playerArray[i]->player_amountOfGold, gameGrid->goldRemaining);
            message_send(gameGrid->playerArray[i]->player_address, (const char*)goldMessage);
            player_updateVisibility(gameGrid->playerArray[i], gameGrid);
            
            char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
            sprintf(displayMessage, "DISPLAY\n%s", gameGrid->playerArray[i]->player_seen);
            message_send(gameGrid->playerArray[i]->player_address, (const char*)displayMessage);

  
          }
        }
      } else {
          // int to keep track of how big new pile of gold is
        int goldCollected = movingPlayer->player_amountOfGold - goldBeforeMove;
        
        printf("\n\n\n\n%d\n\n\n", goldCollected);

        int goldPickedUp = 0;
        for (int i = 0; i < MaxPlayers+1; i++) {
          if (gameGrid->playerArray[i] != NULL) {
            goldPickedUp += gameGrid->playerArray[i]->player_amountOfGold;
          }
        }
        printf("\n\n\n%d\n\n\n", goldPickedUp);
        gameGrid->goldRemaining = GoldTotal - goldPickedUp;

          // send the updated gold message to player who picked up gold
        char goldMessage[100];
        sprintf(goldMessage, "GOLD %d %d %d", goldCollected, movingPlayer->player_amountOfGold, gameGrid->goldRemaining);
        message_send(movingPlayer->player_address, (const char*)goldMessage);

          // looping through all players and updating the new gold informations
        for (int i = 0; i < (MaxPlayers + 1); i++) {

          if (gameGrid->playerArray[i] == NULL) {
            continue;
          }
            // since we have already updated moving player we dont need to do it again
          if (!message_eqAddr(gameGrid->playerArray[i]->player_address, movingPlayer->player_address)) {

            if (gameGrid->playerArray[i]->player_isSpectator) {

              char goldMessage[100];
              sprintf(goldMessage, "GOLD %d %d %d", 0, 0, gameGrid->goldRemaining);
              message_send(gameGrid->playerArray[i]->player_address, (const char*)goldMessage);

              player_updateSpecVisibility(gameGrid->playerArray[i], gameGrid);
              char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
              sprintf(displayMessage, "DISPLAY\n%s", gameGrid->playerArray[i]->player_seen);
              message_send(gameGrid->playerArray[i]->player_address, (const char*)displayMessage);

            } else if (gameGrid->playerArray[i]->player_isActivate){
              char goldMessage[100];
              sprintf(goldMessage, "GOLD %d %d %d", 0, gameGrid->playerArray[i]->player_amountOfGold, gameGrid->goldRemaining);
              message_send(gameGrid->playerArray[i]->player_address, (const char*)goldMessage);

              player_updateVisibility(gameGrid->playerArray[i], gameGrid);
              char displayMessage[(gameGrid->numRows * gameGrid->numColumns) + strlen("DISPLAY\n") + 1];
              sprintf(displayMessage, "DISPLAY\n%s", gameGrid->playerArray[i]->player_seen);
              message_send(gameGrid->playerArray[i]->player_address, (const char*)displayMessage);
            }
          }
        }
      }
    }
  }
  if (gameGrid->goldRemaining == 0) {
      // get the end game stats
    char endGameStats[2701];  // 
    for (int i = 0; i < (MaxPlayers + 1); i++) {
      if (gameGrid->playerArray[i] != NULL) {
        if (!gameGrid->playerArray[i]->player_isSpectator) {
          player_t* getPlayerStats = gameGrid->playerArray[i];

          char playerStats[100];  // needs to be longer, but this is def overkill
          sprintf(playerStats, "%c   %d %s\n", getPlayerStats->player_letter, getPlayerStats->player_amountOfGold, getPlayerStats->player_name);
          printf("%s\n", getPlayerStats->player_name);
          strcat(endGameStats, playerStats);
        }
    
      }
    }

      //    iterate through all players and message_send QUIT using endGame_helper
    for (int i = 0; i < (MaxPlayers + 1); i++) {

      // only sending quit message to active players who != NULL
      if (gameGrid->playerArray[i] != NULL || gameGrid->playerArray[i]->player_isActivate) {
      char quitMessage[2720];
      sprintf(quitMessage, "QUIT GAME OVER:\n%s", endGameStats);
      message_send(gameGrid->playerArray[i]->player_address, (const char*)quitMessage);
      }

    }
    grid_delete(gameGrid);  // deletes all the players
    return true;
  }
  return false;  // start the loop over
}

static char assignLetter(int arrayIdx) {
  if (arrayIdx == 0) {
    return 'A';
  } else if (arrayIdx == 1) {
    return 'B';
  } else if (arrayIdx == 2) {
    return 'C';
  } else if (arrayIdx == 3) {
    return 'D';
  } else if (arrayIdx == 4) {
    return 'E';
  } else if (arrayIdx == 5) {
    return 'F';
  } else if (arrayIdx == 6) {
    return 'G';
  } else if (arrayIdx == 7) {
    return 'H';
  } else if (arrayIdx == 8) {
    return 'I';
  } else if (arrayIdx == 9) {
    return 'J';
  } else if (arrayIdx == 10) {
    return 'K';
  } else if (arrayIdx == 11) {
    return 'L';
  } else if (arrayIdx == 12) {
    return 'M';
  } else if (arrayIdx == 13) {
    return 'N';
  } else if (arrayIdx == 14) {
    return 'O';
  } else if (arrayIdx == 15) {
    return 'P';
  } else if (arrayIdx == 16) {
    return 'Q';
  } else if (arrayIdx == 17) {
    return 'R';
  } else if (arrayIdx == 18) {
    return 'S';
  } else if (arrayIdx == 19) {
    return 'T';
  } else if (arrayIdx == 20) {
    return 'U';
  } else if (arrayIdx == 21) {
    return 'V';
  } else if (arrayIdx == 22) {
    return 'W';
  } else if (arrayIdx == 23) {
    return 'X';
  } else if (arrayIdx == 24) {
    return 'Y';
  } else {
    return 'Z';
  }
}


