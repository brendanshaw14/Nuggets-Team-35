/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef struct player {
    int player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen; 
} player_t; 

static bool checkValidPosition(int position, char* map);
static int isReachBound(int position, int width, int height, char bound);  
static void moveToNewPosition(player_t* player, int newPosition, char* map); 



char* player_getName(player_t* player) {
    return player->player_name; 
}

int player_getPosition(player_t* player) {
    return player->player_position; 
} 

int player_getGold(player_t* player) {
    return player->player_amountOfGold; 
} 

bool player_move(player_t* player, char k, int width, int height, char* map) {
    // TODO: add invalid check for k
    // ....
    switch (k)
    {
    case 'h':
        // move left
        // if player already reaches the left boundary
        if (isReachBound(player->player_position, width, height, 'l') == 1) {
            return true; 
        } 
        // move to new position
        moveToNewPosition(player, player->player_position - 1, map); 
        break;
    case 'l':
        // move right
        // if player already reaches the right boundary
        if (isReachBound(player->player_position, width, height, 'r') == 1) {
            return true; 
        }
        // move to new position
        moveToNewPosition(player, player->player_position + 1, map); 
        break; 
    case 'j':
        // move down
        // if player already reaches the bottom boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1) {
            return true; 
        }
        // move to new position
        moveToNewPosition(player, player->player_position + width, map); 
        break; 
    case 'k':
        // move up
        // if player already reaches the top boundary
        if (isReachBound(player->player_position, width, height, 't') == 1) {
            return true; 
        }
        // move to new position
        moveToNewPosition(player, player->player_position - width, map); 
        break; 
    case 'y': 
        // move diagonally up and left
        // if player already reaches the top or left boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        // move to new position
        moveToNewPosition(player, player->player_position - width - 1, map); 
        break; 
    case 'u':
        // move diagonally up and right
        // if player already reaches the top or right boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        // move to new position
        moveToNewPosition(player, player->player_position - width + 1, map); 
        break; 
    case 'b': 
        // move diagonally down and left
        // if player already reaches the bottom or left boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        // move to new position
        moveToNewPosition(player, player->player_position + width - 1, map); 
        break; 
    case 'n':
        // move diagonally down and right
        // if player already reaches the bottom or right boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        // move to new position
        moveToNewPosition(player, player->player_position + width + 1, map); 
        break; 
    default:
        break;
    }
    return true; 
}

static int isReachBound(int position, int width, int height, char bound) {
    switch (bound)
    {
    case 't':
        // top bound
        if (position / width == 0) {
            return 1; 
        } else {
            return 0; 
        }
        break;
    case 'b':
        // bottom bound
        if (position / width == height - 1) {
            return 1; 
        } else {
            return 0; 
        }
        break; 
    case 'l':
        // left bound
        if (position % width == 0) {
            return 1; 
        } else {
            return 0; 
        }
        break; 
    case 'r':
        // right bound
        if (position % width == width - 1) {
            return 1; 
        } else {
            return 0; 
        }
        break; 
    default:
        break;
    }
    // invalid input
    fprintf(stderr, "Error: Invaild input when checking bound ...\n"); 
    return -1; 
}

static void moveToNewPosition(player_t*player, int newPosition, char* map) {
    // if new position is reachable, then move to it
    // otherwise, do nothing
    if (checkValidPosition(newPosition, map)) {
        player->player_position = newPosition; 
        // TODO: change the "seen" string, better to define another function
    }
}

static bool checkValidPosition(int position, char* map) {
    if (map[position] == '.' || map[position] == '#') {
        return true; 
    } 
    return false; 
}