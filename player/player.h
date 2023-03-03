/* Player.h - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

// includes
#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../grid/grid.h"
#include "../support/message.h"

/****************global types**************/
typedef struct grid grid_t;

typedef struct player {
    addr_t player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    int player_visibility_range; 
    char* player_seen; 
    hashtable_t* player_passageVisited; 
    bool player_isSpectator; 
    char player_letter;
} player_t; 

/***********player_init*************/
/* Initializes a new player given a grid and other player information. 
    E.g.: player address, name, visibility range, letter, whether is spectator
    - sets the player address, name, letter, visibility range, amount of gold found
    - initializes the player "seen" string
    - initializes a hashtable storing the passage it has visited 
    - sets the blockCharList storing the chars that will block vision 
Requires:
    - pointer to grid, type: grid_t*
    - address, type: addr_t
    - name, type: char*
    - isSpectator, type: bool
    - radius, type: int
    - letter, type: char
Returns: 
    - a pointer to player
*/
player_t* player_init(grid_t* grid, addr_t address, char* name, bool isSpectator, int radius, char letter); 

/***********player_delete*************/
/* Delete a player.
    - searchs in the playerArray in grid to find certain player
    - matches the player by comparing address
    - sets the item in playerArray to be NULL
Requires:
    - pointer to player, type: player_t*
    - pointer to grid, type: grid_t*
Returns: 
    - None
*/
void player_delete(player_t* player, grid_t* grid); 

/***********player_getName*************/
/* Returns player name.
Requires:
    - pointer to player, type: player_t*
Returns: 
    - player name
*/
char* player_getName(player_t* player);

/***********player_getPosition*************/
/* Returns player position.
Requires:
    - pointer to player, type: player_t*
Returns: 
    - player position
*/
int player_getPosition(player_t* player); 

/***********player_getGold*************/
/* Returns the amount of gold this player has found.
Requires:
    - pointer to player, type: player_t*
Returns: 
    - amount of gold
*/
int player_getGold(player_t* player); 

/***********player_getVisibility*************/
/* Returns the "seen" string of this player 
Requires:
    - pointer to player, type: player_t*
Returns: 
    - the "seen" string of this player 
*/
char* player_getVisibility(player_t* player);

/***********player_move*************/
/* Moves the player.
    - parses the input char
    - check if the player is reaching boundary
        - if so, directly return true
        - else, compute the new position
    - move the player by calling "moveToNewPosition" function
    - update player visibility
    - update player_amountOfGold
    - update the playerArray in grid
    - switch the player if needed, and steal the gold 
Requires:
    - pointer to player, type: player_t*
    - pointer to grid, type: grid_t*
    - input char
Returns: 
    - true: valid input char
    - false: invalid input char
*/
bool player_move(player_t* player, grid_t* grid, char k); 

/***********player_updateVisibility*************/
/* Update what this player can see.
    TODO .....
    - 
Requires:
    - 
Returns: 
    - 
*/
void player_updateVisibility(player_t* player, grid_t* grid); 

void player_updateSpecVisibility(player_t* player, grid_t* grid); 

#endif