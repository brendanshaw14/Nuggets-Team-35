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
/* Update what this regular player can see.
    - a rectangle shape of range is implemented
    - loop from [x - radius, x + radius], [y - radius, y + radius]:
        - for each valid position,
            - if in the same row with player:
                - call "updateVisibilitySameRow" function to update visibility
                - show other player if exists
            - if in the same column with player:
                - call "updateVisibilitySameCol" function to update visibility
                - show other player if exists
            - otherwise:
                - for each reachable position (call "isRoom" to check):
                    - "isRoom" function does: 
                        - computes the slope of line and calls functions below to check intersection
                        - calls "checkVertical" to check if there's any vertical wall
                        - calls "checkHorizonal" to check if there's any horizonal wall
                        - current position is visible if there's no both two kinds of walls 
                    - update visibility
                    - show other player if exists
                    - hide gold mark '*' for regular player
Requires:
    - pointer to player, type: player_t*
    - pointer to grid, type: grid_t*
Returns: 
    - None
Note:
    - for each position in the range, it's visible for the player only if: 
        - there's no vertical wall during the path between player and itself
        - there's no horizonal wall during the path between player and itself
    - we consider "same row", "same column", "other cases" respectively here, 
        - for the sake of any "devided by 0" error when calculating line slope
    - some extra credit is supported here: 
        - limited range
        - different player can have different size of visibility range
*/
void player_updateVisibility(player_t* player, grid_t* grid); 

/***********player_updateSpecVisibility*************/
/* Update what the spectator can see.
    - loop the whole seen string of spectator,
        - visualize all players
        - if there used to be a gold but there's no gold now (already collected by some players), 
            we hide '*' for spectator
Requires:
    - pointer to player, type: player_t*
    - pointer to grid, type: grid_t*
Returns: 
    - None
*/
void player_updateSpecVisibility(player_t* player, grid_t* grid); 

#endif