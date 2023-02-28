/* Player.h - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

// includes
#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"

typedef struct player player_t; 

typedef struct player {
    int player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen; 
    hashtable_t* player_passageVisited; 
    bool player_isSpectator; 
} player_t;

player_t* player_init(char* map, int address, int init_position, char* name, bool isSpectator); 

char* player_getName(player_t* player);

int player_getPosition(player_t* player); 

int player_getGold(player_t* player); 

char* player_getVisibility(player_t* player);

bool player_move(player_t* player, char k, int width, int height, char* map, int radius); 

void player_updateVisibility(player_t* player, char* map, int width, int height, int radius); 

#endif