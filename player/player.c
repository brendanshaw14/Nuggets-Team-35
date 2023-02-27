/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <math.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

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
static int getRow(int position, int width); 
static int getCol(int position, int width); 
static double computeDistance(int x1, int y1, int x2, int y2);
static bool isInteger(double a); 
static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width); 
static int convertToIndex(int row, int col, int width); 
static bool checkVertical(char* map, int startCol, int endCol, int startRow, int endRow, int width, double k); 
static bool checkHorizonal(char* map, int startCol, int endCol, int startRow, int endRow, int width, double k); 
static void swap(int* a, int* b); 

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

void player_updateVisibility(player_t* player, char* map, int width, int height, int radius) {
    // consider '\n' before computing 
    width++; 
    // get x and y coordinates 
    int playerRow = getRow(player->player_position, width), playerCol = getCol(player->player_position, width);
    printf("curr player row: %d, col: %d\n", playerRow, playerCol); 
    // loop all positions in the range [x - radius, x + radius], [y - radius, y + radius] 
    int col = playerCol - radius, row = playerRow - radius; 
    printf("curr col init: %d, curr row init: %d\n", col, row); 
    for (col = playerCol - radius; col <= playerCol + radius; col++) {
        for (row = playerRow - radius; row <= playerRow + radius; row++) {
            // skip the player position
            if (row == playerRow && col == playerCol) {
                continue; 
            }
            // if exeeds bound, continue
            if (row < 0 || row >= height || col < 0 || col >= width) {
                continue; 
            }
            if (col == playerCol) {
                // cannot calculate the slope
                // loop all rows
                int minRow = min(row, playerRow), maxRow = max(row, playerRow); 
                int i = minRow + 1; 
                for (i = minRow + 1; i < maxRow; i++) {
                    int index = convertToIndex(i, col, width); 
                    // if the path is not available
                    if (map[index] != '.') {
                        break; 
                    }
                }
                // if all dots along the path is visible
                if (i == maxRow) {
                    int index = convertToIndex(row, col, width); 
                    player->player_seen[index] = map[index]; 
                }
            } else {
                printf("curr col: %d, curr row: %d\n", col, row); 
                if (isRoom(col, row, playerCol, playerRow, map, width)) {
                    printf("visible row: %d, visible col: %d\n", row, col); 
                    // current position [col, row] can be visible
                    int index = convertToIndex(row, col, width); 
                    player->player_seen[index] = map[index]; 
                }
            }
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
            player_updateVisibility(player, map, width, height, radius); 
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

static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width) {
    double k = (playerRow - currRow + 0.0) / (playerCol - currCol + 0.0); 
    int startCol = min(playerCol, currCol), endCol = max(playerCol, currCol); 
    int startRow = 0, endRow = 0; 
    if (k > 0) {
        // k > 0: 
        // player is in the diag up-left or bottom-right direction
        startRow = min(playerRow, currRow); 
        endRow = max(playerRow, currRow); 
    } else {
        // k < 0: 
        // player is in the diag up-right or bottom-left direction
        startRow = max(playerRow, currRow); 
        endRow = min(playerRow, currRow); 
    } 
    printf("start row: %d, start col: %d, end col: %d\n", startRow, startCol, endCol); 

    bool vertical = checkVertical(map, startCol, endCol, startRow, endRow, width, k); 
    bool horizonal = checkHorizonal(map, startCol, endCol, startRow, endRow, width, k);

    return vertical && horizonal; 
}

static bool checkVertical(char* map, int startCol, int endCol, int startRow, int endRow, int width, double k) {
    for (int col = startCol + 1; col < endCol; col++) {
        double row = startRow + k * (col - startCol + 0.0); 

        // check if current row is an int or not
        if (isInteger(row)) {
            // current row is actually an int, no need to check it's neighbours
            // convert from [col, row] to index
            int index = convertToIndex(row, col, width);  
            return map[index] == '.';   
        } else {
            // current row is actually a floating number, check it's neighbours
            // e.g.: (5, 5.2) -> check (5, 5) and (5, 6)
            int nei1 = floor(row), nei2 = nei1 + 1; 
            // convert from (col, row) to index
            int index1 = convertToIndex(nei1, col, width), index2 = convertToIndex(nei2, col, width); 
            // if at least one neighbour is room, current position is room
            return map[index1] == '.' || map[index2] == '.'; 
        }
    }
    return true; 
}

static bool checkHorizonal(char* map, int startCol, int endCol, int startRow, int endRow, int width, double k) {
    // k > 0: startRow < endRow
    // k < 0: startRow > endRow, we need to swap before for loop
    if (k < 0) {
        printf("start row should be greater than end row.. start: %d, end: %d\n", startRow, endRow); 
        swap(&startRow, &endRow); 
        printf("after swapping.... start: %d, end: %d\n", startRow, endRow);
    }
    for (int row = startRow + 1; row < endRow; row++) {
        double col = startCol + (row - startRow + 0.0) / k; 
        // check if current col is an int or not
        if (isInteger(col)) {
            int index = convertToIndex(row, col,width); 
            return map[index] == '.'; 
        } else {
            int nei1 = floor(col), nei2 = nei1 + 1; 
            int index1= convertToIndex(row, nei1, width), index2 = convertToIndex(row, nei2, width); 
            return map[index1] == '.' || map[index2] == '.'; 
        }
    }
    return true; 
}

static void swap(int* a, int* b) {
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
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
