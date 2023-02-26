/* Player.h - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

// includes
#include <stdio.h>
#include <stdlib.h>

typedef struct player player_t; 

char* player_getName(player_t* player);

int player_getPosition(player_t* player); 

int player_getGold(player_t* player); 

char* player_getVisibility(player_t* player);

bool player_move(player_t* player, char k, int width, int height, char* map, double radius); 

void player_updateVisibility(player_t* player, char* map, int width, double radius); 

#endif