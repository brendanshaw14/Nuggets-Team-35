/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <math.h>
#include "../libcs50/hashtable.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

typedef struct player {
    int player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen; 
    hashtable_t* player_passageVisited; 
    bool player_isSpectator; 
} player_t; 

const char player_mark = '*'; 
const char init_seen_mark = ' '; 
const int num_slots = 20; 

hashtable_t* blockCharList; 

static bool checkValidPosition(int position, char* map);
static int isReachBound(int position, int width, int height, char bound);  
static bool moveToNewPosition(player_t* player, int newPosition, char* map); 
static int getRow(int position, int width); 
static int getCol(int position, int width); 
static bool isInteger(double a); 
static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width); 
static int convertToIndex(int row, int col, int width); 
static bool checkVertical(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k); 
static bool checkHorizonal(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k); 
static void updateVisibilitySameCol(player_t* player, char* map, int commonCol, int currRow, int playerRow, int width); 
static void updateVisibilitySameRow(player_t* player, char* map, int commonRow, int currCol, int playerCol, int width);
static bool isLastPassage(player_t* player, char* map, int currIndex, int width);  
static char* intToString(int a); 
static char* charToString(char a); 

void player_updateVisibility(player_t* player, char* map, int width, int height, int radius) {
    // consider '\n' before computing 
    width++; 
    // get x and y coordinates 
    int playerRow = getRow(player->player_position, width), playerCol = getCol(player->player_position, width);

    // if player is in '#' currently: 
        // if it's not the last passage:
            // set radius to be 1 
        // else :
            // radius remains the same 
    int index = convertToIndex(playerRow, playerCol, width); 
    if (map[index] == '#') {
        char* indexStr = intToString(index); 
        hashtable_insert(player->player_passageVisited, indexStr, "");
        if (!isLastPassage(player, map, index, width)) {
            radius = 1; 
        }
    } 

    // loop all positions in the range [x - radius, x + radius], [y - radius, y + radius] 
    for (int col = playerCol - radius; col <= playerCol + radius; col++) {
        for (int row = playerRow - radius; row <= playerRow + radius; row++) {
            // skip the player position
            if (row == playerRow && col == playerCol) {
                continue; 
            }
            // if exeeds bound, continue
            if (row < 0 || row >= height || col < 0 || col >= width) {
                continue; 
            }
            if (col == playerCol) {
                // loop all rows
                updateVisibilitySameCol(player, map, col, row, playerRow, width); 
                
            } else if (row == playerRow) {
                // loop all cols
                updateVisibilitySameRow(player, map, row, col, playerCol, width); 
            } else {
                // diag direction 
                if (isRoom(col, row, playerCol, playerRow, map, width)) {
                    // current position [col, row] can be visible
                    int index = convertToIndex(row, col, width); 
                    player->player_seen[index] = map[index]; 
                }
            }
        }
    }
}

player_t* player_init(char* map, int address, int init_position, char* name, bool isSpectator, int width, int height, int radius) {
    player_t* player = malloc(sizeof(player_t)); 
    player->player_address = address; 
    player->player_position = init_position; 
    player->player_name = name; 
    player->player_amountOfGold = 0;
    player->player_passageVisited = hashtable_new(num_slots);  
    player->player_isSpectator = isSpectator;  
    player->player_seen = malloc(strlen(map)); 
    // initialize player_seen string
    for (int i = 0; i < strlen(map); i++) {
        if (map[i] == '\n') {
            player->player_seen[i] = '\n'; 
        } else {
            player->player_seen[i] = init_seen_mark; 
        }
    }
    // mark the player 
    player->player_seen[init_position] = player_mark; 
    // set the block char
    blockCharList = hashtable_new(num_slots); 
    hashtable_insert(blockCharList, "|", ""); 
    hashtable_insert(blockCharList, "+", ""); 
    hashtable_insert(blockCharList, "#", ""); 
    hashtable_insert(blockCharList, "-", "");
    hashtable_insert(blockCharList, " ", ""); 

    // visualize the init range of player
    player_updateVisibility(player, map, width, height, radius);

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

bool player_move(player_t* player, char k, int width, int height, char* map, int radius) {
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
            player_updateVisibility(player, map, width, height, radius); 
            // TODO: if new position is gold, update player_amountOfGold, better define another function to do this
        }
        break;
    case 'l':
        // move right
        // if player already reaches the right boundary
        if (isReachBound(player->player_position, width, height, 'r') == 1) {
            return true; 
        }
        // move to new position
        if (moveToNewPosition(player, player->player_position + 1, map)) {
            // update the "seen" string
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
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
            player_updateVisibility(player, map, width, height, radius); 
        }
        break; 
    default:
        break;
    }
    return true; 
}

static bool isLastPassage(player_t* player, char* map, int currIndex, int width) {
    // check 4 directions
    // if there's at least 1 direction is '#' (not previously seen), return false
    // else, return true
    int leftIndex = currIndex - 1, rightIndex = currIndex + 1, 
        upIndex = currIndex - width, downIndex = currIndex + width; 

    char *leftIndexStr = intToString(leftIndex), *rightIndexStr = intToString(rightIndex), 
        *upIndexStr = intToString(upIndex), *downIndexStr = intToString(downIndex); 

    if (leftIndex >= 0 && map[leftIndex] == '#' && hashtable_find(player->player_passageVisited, leftIndexStr) == NULL) {
        return false; 
    } 
    if (rightIndex < strlen(map) && map[rightIndex] == '#' && hashtable_find(player->player_passageVisited, rightIndexStr) == NULL) {
        return false; 
    } 
    if (upIndex >= 0 && map[upIndex] == '#' && hashtable_find(player->player_passageVisited, upIndexStr) == NULL) {
        return false; 
    } 
    if (downIndex < strlen(map) && map[downIndex] == '#' && hashtable_find(player->player_passageVisited, downIndexStr) == NULL) {
        return false; 
    } 
    return true; 
}

static char* intToString(int a) {
    char* str = malloc(80);  
    sprintf(str, "%d", a); 
    return str; 
}

static char* charToString(char a) {
    char* str = malloc(1); 
    str[0] = a; 
    return str; 
}

static void updateVisibilitySameCol(player_t* player, char* map, int commonCol, int currRow, int playerRow, int width) {
    int minRow = min(currRow, playerRow), maxRow = max(currRow, playerRow); 
    int i = minRow + 1; 
    for (i = minRow + 1; i < maxRow; i++) {
        int index = convertToIndex(i, commonCol, width); 
        // if the path is not available
        if (map[index] != '.') {
            break; 
        }
    }
    // if all dots along the path is visible
    if (i == maxRow) {
        int index = convertToIndex(currRow, commonCol, width); 
        player->player_seen[index] = map[index]; 
    }
}

static void updateVisibilitySameRow(player_t* player, char* map, int commonRow, int currCol, int playerCol, int width) {
    int minCol = min(currCol, playerCol), maxCol = max(currCol, playerCol); 
    int i = minCol + 1; 
    for (i = minCol + 1; i < maxCol; i++) {
        int index = convertToIndex(commonRow, i, width); 
        if (map[index] != '.') {
            break; 
        }
    }
    // if all dots along the path is visible
    if (i == maxCol) {
        int index = convertToIndex(commonRow, currCol, width); 
        player->player_seen[index] = map[index]; 
    }
}

static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width) {
    double k = (playerRow - currRow + 0.0) / (playerCol - currCol + 0.0); 

    bool vertical = checkVertical(map, playerCol, playerRow, currCol, currRow, width, k); 
    bool horizonal = checkHorizonal(map, playerCol, playerRow, currCol, currRow, width, k); 

    return vertical && horizonal; 
}

static bool checkVertical(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k) {

    int startCol = 0, endCol = 0, startRow = 0; 

    if (currCol < playerCol) {
        startCol = currCol; 
        endCol = playerCol; 
        startRow = currRow; 
    } else {
        startCol = playerCol; 
        endCol = currCol; 
        startRow = playerRow; 
    }

    for (int col = startCol + 1; col < endCol; col++) {
        double row = startRow + k * (col - startCol + 0.0); 

        // check if current row is an int or not
        if (isInteger(row)) {
            // current row is actually an int, no need to check it's neighbours
            // convert from [col, row] to index
            int index = convertToIndex(row, col, width);  
            if (hashtable_find(blockCharList, charToString(map[index]))) {
                return false; 
            }  
        } else {
            // current row is actually a floating number, check it's neighbours
            // e.g.: (5, 5.2) -> check (5, 5) and (5, 6)
            int nei1 = floor(row), nei2 = nei1 + 1; 
            // convert from (col, row) to index
            int index1 = convertToIndex(nei1, col, width), index2 = convertToIndex(nei2, col, width); 
            if (hashtable_find(blockCharList, charToString(map[index1])) && hashtable_find(blockCharList, charToString(map[index2]))) {
                return false; 
            }
        }
    }
    return true; 
}

static bool checkHorizonal(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k) {
    int startRow = 0, endRow = 0, startCol = 0; 

    if (currRow < playerRow) {
        startRow = currRow; 
        endRow = playerRow; 
        startCol = currCol; 
    } else {
        startRow = playerRow; 
        endRow = currRow; 
        startCol = playerCol; 
    }
    
    for (int row = startRow + 1; row < endRow; row++) {
        double col = startCol + (row - startRow + 0.0) / k; 
        // check if current col is an int or not
        if (isInteger(col)) {
            int index = convertToIndex(row, col,width); 
            if (hashtable_find(blockCharList, charToString(map[index]))) {
                return false; 
            } 
        } else {
            int nei1 = floor(col), nei2 = nei1 + 1; 
            int index1= convertToIndex(row, nei1, width), index2 = convertToIndex(row, nei2, width); 
            if (hashtable_find(blockCharList, charToString(map[index1])) && hashtable_find(blockCharList, charToString(map[index2]))) {
                return false; 
            } 
        }
    }
    return true; 
}

static int convertToIndex(int row, int col, int width) {
    return row * width + col; 
}

static bool isInteger(double a) {
    int b = a; 
    double diff = a - b; 
    return diff == 0; 
}

static int getRow(int position, int width) {
    return position / width; 
}

static int getCol(int position, int width) {
    return position % width;  
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
