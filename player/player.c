/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <math.h>

typedef struct player {
    int player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen; 
    bool player_isSpectator; 
} player_t; 

const char player_mark = '*'; 
const char init_seen_mark = '!'; 

static bool checkValidPosition(int position, char* map);
static int isReachBound(int position, int width, int height, char bound);  
static bool moveToNewPosition(player_t* player, int newPosition, char* map); 
static int getXAxis(int position, int width); 
static int getYAxis(int position, int width); 
static double computeDistance(int x1, int y1, int x2, int y2);

player_t* player_init(char* map, int address, int init_position, char* name, bool isSpectator) {
    player_t* player = malloc(sizeof(player_t)); 
    player->player_address = address; 
    player->player_position = init_position; 
    player->player_name = name; 
    player->player_amountOfGold = 0;
    player->player_isSpectator = isSpectator;  
    player->player_seen = malloc(strlen(map)); 
    // initialize player_seen string
    // TODO: do we need to visualize the init range of player? 
    for (int i = 0; i < strlen(map); i++) {
        if (map[i] == '\n') {
            player->player_seen[i] = '\n'; 
        } else {
            player->player_seen[i] = init_seen_mark; 
        }
    }
    // mark the player 
    player->player_seen[init_position] = player_mark; 
    return player; 
}

char* player_getName(player_t* player) {
    return player->player_name; 
}

int player_getPosition(player_t* player) {
    return player->player_position; 
} 

int player_getGold(player_t* player) {
    return player->player_amountOfGold; 
} 

char* player_getVisibility(player_t* player) {
    return player->player_seen; 
}

void player_updateVisibility(player_t* player, char* map, int width, double radius) {
    // consider '\n' before computing 
    width++; 
    // get x and y coordinates 
    int playerXCoord = getXAxis(player->player_position, width), playerYCoord = getYAxis(player->player_position, width); 
    for (int position = 0; position < strlen(map); position++) {
        // get x and y coordinates 
        int currXCoord = getXAxis(position, width), currYCoord = getYAxis(position, width); 
        // compute distance
        double distance = computeDistance(currXCoord, currYCoord, playerXCoord, playerYCoord); 
        if (distance < pow(radius, 2) && player->player_seen[position] == init_seen_mark) {
            // if current position is within the circle and it's not been seen yet
            // set it to map[position]
            player->player_seen[position] = map[position]; 
        }
    }
}

bool player_move(player_t* player, char k, int width, int height, char* map, double radius) {
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
        if (moveToNewPosition(player, player->player_position - 1, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius); 
            // TODO: if new position is gold, update player_amountOfGold, better define another function to do this
        }
        break;
    case 'l':
        // move right
        // if player already reaches the right boundary
        printf("position: %d, width: %d, height: %d\n", player->player_position, width, height); 
        if (isReachBound(player->player_position, width, height, 'r') == 1) {
            return true; 
        }
        printf("Begin to move ....\n"); 
        // move to new position
        if (moveToNewPosition(player, player->player_position + 1, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius); 
        } 
        break; 
    case 'j':
        // move down
        // if player already reaches the bottom boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1) {
            return true; 
        }
        // move to new position
        if (moveToNewPosition(player, player->player_position + width + 1, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius); 
        }
        break; 
    case 'k':
        // move up
        // if player already reaches the top boundary
        if (isReachBound(player->player_position, width, height, 't') == 1) {
            return true; 
        }
        // move to new position
        if (moveToNewPosition(player, player->player_position - width - 1, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius);
        }
        break; 
    case 'y': 
        // move diagonally up and left
        // if player already reaches the top or left boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        // move to new position
        if (moveToNewPosition(player, player->player_position - width - 2, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius);
        }
        break; 
    case 'u':
        // move diagonally up and right
        // if player already reaches the top or right boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        // move to new position
        if (moveToNewPosition(player, player->player_position - width, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius);
        }
        break; 
    case 'b': 
        // move diagonally down and left
        // if player already reaches the bottom or left boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        // move to new position
        if (moveToNewPosition(player, player->player_position + width, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius);
        }
        break; 
    case 'n':
        // move diagonally down and right
        // if player already reaches the bottom or right boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        // move to new position
        if (moveToNewPosition(player, player->player_position + width + 2, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, radius);
        }
        break; 
    default:
        break;
    }
    return true; 
}

static int getXAxis(int position, int width) {
    return position / width; 
}

static int getYAxis(int position, int width) {
    int currRow = position / width; 
    return position % width - currRow;  
}

// We define a function here in case that we can use different methods to compute distance
// Currently is: (x1 - x2)^2 + (y1 - y2)^2 
static double computeDistance(int x1, int y1, int x2, int y2) {
    return pow(x1 - x2, 2) + pow(y1 - y2, 2); 
}

static int isReachBound(int position, int width, int height, char bound) {
    // consider '\n' before computing
    width++; 
    
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

static bool moveToNewPosition(player_t*player, int newPosition, char* map) {
    // if new position is reachable, then move to it
    // otherwise, do nothing
    if (checkValidPosition(newPosition, map)) {
        // reset the originl position because we're begin to move
        player->player_seen[player->player_position] = map[player->player_position]; 
        player->player_position = newPosition; 
        player->player_seen[player->player_position] = player_mark; 
        return true; 
    }
    return false; 
}

static bool checkValidPosition(int position, char* map) {
    if (map[position] == '.' || map[position] == '#') {
        return true; 
    } 
    return false; 
}
