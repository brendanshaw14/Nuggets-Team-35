/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <math.h>
#include "../libcs50/hashtable.h"
#include "../grid/grid.h"
#include "../support/message.h"

// definition of "max" and "min" function
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// definition of constants 
const char player_mark = '@'; 
const char init_seen_mark = ' '; 
const char gold_mark = '*'; 
const char available_mark = '.'; 
const int num_slots = 20; 
const int max_player_number = 26; 
const double steal_percentage = 0.5; 

// delete hashtable storing visited passage for a player
void passageVisitedDelete(void* item);
// check whether current position is valid so that player can move to
static bool checkValidPosition(int position, char* map);
// check whether current position reaches the boundary of the map
static int isReachBound(int position, int width, int height, char bound);  
// move the player
static bool moveToNewPosition(player_t* player, int newPosition, char* map); 
// return the row number of current position
static int getRow(int position, int width); 
// return the column number of current position
static int getCol(int position, int width); 
// check whether a floating number is actually equal to an int
static bool isInteger(double a); 
// check whether current position is a room (
    // not blocked by walls during the path from it to player)
static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width); 
// convert from [row, col] to index in the map
static int convertToIndex(int row, int col, int width); 
// check vertical walls existence
static bool checkVertical(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k); 
// check horizonal walls existence
static bool checkHorizonal(char* map, int playerCol, int playerRow, int currCol, int currRow, int width, double k); 
// update the visibility when the position is at the same column with player
static void updateVisibilitySameCol(player_t* player, char* map, int commonCol, int currRow, int playerRow, int width); 
// update the visibility when the position is at the same row with player
static void updateVisibilitySameRow(player_t* player, char* map, int commonRow, int currCol, int playerCol, int width);
// check whether it's the last passage
static bool isLastPassage(player_t* player, char* map, int currIndex, int width);  
// check whether current position is a player
static int isPlayer(grid_t* grid, int position); 
// update amount of gold by current player
static void updateGoldAmount(player_t* player, grid_t* grid); 
// update the playerArray in grid
static void updatePlayerArray(player_t* player, grid_t* grid); 
// check if char is one of the following: '|', '+', '#', '-', ' '
static bool isBlockChar(char ch); 
// convert from int to string
static char* intToString(int a); 

// see player.h for more information
void player_updateVisibility(player_t* player, grid_t* grid) {
    char* map = grid->gridString; 
    int width = grid->numColumns, height = grid->numRows, radius = player->player_visibility_range; 
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
        free(indexStr);
    } 

    // loop all positions in the range [x - radius, x + radius], [y - radius, y + radius] 
    for (int col = playerCol - radius; col <= playerCol + radius; col++) {
        for (int row = playerRow - radius; row <= playerRow + radius; row++) {
            // skip the player position
            if (row == playerRow && col == playerCol) {
                int index = convertToIndex(row, col, width); 
                // mark the player itself as '@' 
                player->player_seen[index] = player_mark; 
                continue; 
            }
            // if exeeds bound, continue
            if (row < 0 || row >= height || col < 0 || col >= width) {
                continue; 
            }
            if (col == playerCol) {
                // loop all rows
                updateVisibilitySameCol(player, map, col, row, playerRow, width); 
                // if there's another player, show it
                int index = convertToIndex(row, col, width); 
                int locationInArray = isPlayer(grid, index); 
                if (locationInArray != -1) {
                    // set the other player as player_letter
                    player->player_seen[index] = grid->playerArray[locationInArray]->player_letter;
                }
            } else if (row == playerRow) {
                // loop all cols
                updateVisibilitySameRow(player, map, row, col, playerCol, width); 
                // if there's another player, show it
                int index = convertToIndex(row, col, width); 
                int locationInArray = isPlayer(grid, index); 
                if (locationInArray != -1) {
                    // set the other player as player_letter
                    player->player_seen[index] = grid->playerArray[locationInArray]->player_letter;
                }
            } else {
                // diag direction 
                if (isRoom(col, row, playerCol, playerRow, map, width)) {
                    // current position [col, row] can be visible
                    int index = convertToIndex(row, col, width); 
                    // set this position to be visible 
                    player->player_seen[index] = map[index]; 
                    // if there's another player in this position, show it
                    int locationInArray = isPlayer(grid, index); 
                    if (locationInArray != -1) {
                        // set the other player as player_letter
                        player->player_seen[index] = grid->playerArray[locationInArray]->player_letter;
                    }
                }
            }
        }
    }
}

// see player.h for more information
void player_updateSpecVisibility(player_t* player, grid_t* grid) {
    // as for the spectator, it should show all players in the map
    char* map = grid->gridString; 
    // loop the whole seen string, and visualize all players 
    for (int i = 0; i < strlen(map); i++) {
        int locationInArray = isPlayer(grid, i); 
        if (locationInArray != -1) {
            // current index is player, set it as player 
            // set the player as player_letter
            player->player_seen[i] = grid->playerArray[locationInArray]->player_letter;
        } else {
            // current index is not player, set is the same as map
            player->player_seen[i] = map[i]; 
        }

        // if map[index] == '*' (there used to be a gold) but there's no gold now (already collected by some players), 
        // we should not show '*' for spectator 
        // but if there's a player now (the player is collecting gold, we should show the player)
        // e.g.:
        // ..P*. -> ...P. -> ....P
        if (counters_get(grid->goldTable, i) == 0 && map[i] == gold_mark && isPlayer(grid, i) == -1) {
            player->player_seen[i] = available_mark; 
        }
    }

} 

// see player.h for more information
player_t* player_init(grid_t* grid, addr_t address, char* name, bool isSpectator, int radius, char letter) {
    char* map = grid->gridString; 
    player_t* player = malloc(sizeof(player_t)); 
    player->player_address = address; 
    player->player_position = 0; 
    player->player_name = name; 
    player->player_amountOfGold = 0;
    player->player_passageVisited = hashtable_new(num_slots);  
    player->player_isSpectator = isSpectator;  
    player->player_visibility_range = radius; 
    player->player_seen = malloc(strlen(map) +1); 
    player->player_letter = letter;
    // initialize player_seen string
    if (!isSpectator) {
        // if it's a regular player, we should initialize the string as empty
        for (int i = 0; i < strlen(map); i++) {
            if (map[i] == '\n') {
                player->player_seen[i] = '\n'; 
            } else {
                player->player_seen[i] = init_seen_mark; 
            }
        }
        player -> player_seen[strlen(map)] = '\0';
    } else {
        // if it's spectator, we should initialize the string to be the same as the map
        strcpy(player->player_seen, map); 
    }

    return player; 
}

// see player.h for more information
void player_delete(player_t* player, grid_t* grid) {
    // delete this player in the playerArray in grid
    for (int i = 0; i < max_player_number + 1; i++) {
        // check if a player exists for that i and use address to compare the player 
        if (grid->playerArray[i] != NULL){
            if (message_eqAddr(grid->playerArray[i]->player_address, player->player_address)) {
                // set it to NULL
                hashtable_delete(player->player_passageVisited, NULL);
                grid->playerArray[i] = NULL; 
                free(player -> player_seen);
                free(player);
                break; 
            }
        }
    }
    return;
}

// delete the visited passage of current player
void passageVisitedDelete(void* item){
    return;
}

// return player name
char* player_getName(player_t* player) {
    return player->player_name; 
}

// return player position
int player_getPosition(player_t* player) {
    return player->player_position; 
} 

// return amount of gold player has found
int player_getGold(player_t* player) {
    return player->player_amountOfGold; 
} 

// return "seen" string of player
char* player_getVisibility(player_t* player) {
    return player->player_seen; 
}

// move the player
bool player_move(player_t* player, grid_t* grid, char k) {
    char* map = grid->gridString; 
    int width = grid->numColumns, height = grid->numRows; 
    int newPosition = -1; 
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
        newPosition = player->player_position - 1; 
        break;
    case 'l':
        // move right
        // if player already reaches the right boundary
        if (isReachBound(player->player_position, width, height, 'r') == 1) {
            return true; 
        }
        newPosition = player->player_position + 1; 
        break; 
    case 'j':
        // move down
        // if player already reaches the bottom boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1) {
            return true; 
        }
        newPosition = player->player_position + width + 1; 
        break; 
    case 'k':
        // move up
        // if player already reaches the top boundary
        if (isReachBound(player->player_position, width, height, 't') == 1) {
            return true; 
        }
        newPosition = player->player_position - width - 1; 
        break; 
    case 'y': 
        // move diagonally up and left
        // if player already reaches the top or left boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        newPosition = player->player_position - width - 2; 
        break; 
    case 'u':
        // move diagonally up and right
        // if player already reaches the top or right boundary
        if (isReachBound(player->player_position, width, height, 't') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        newPosition = player->player_position - width; 
        break; 
    case 'b': 
        // move diagonally down and left
        // if player already reaches the bottom or left boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'l') == 1) {
                return true; 
        } 
        newPosition = player->player_position + width; 
        break; 
    case 'n':
        // move diagonally down and right
        // if player already reaches the bottom or right boundary
        if (isReachBound(player->player_position, width, height, 'b') == 1 || 
            isReachBound(player->player_position, width, height, 'r') == 1) {
                return true; 
        } 
        newPosition = player->player_position + width + 2; 
        break; 
    default:
        return false; 
    }

    // Code below does the following: 
        // 1. Move the player 
        // 2. Update the player's visibility
        // 3. Update gold amount if finding a gold
        // 4. Update the playerArray
    
    // in the case of player switch
    int oldPosition = player->player_position; 
    int occupiedPlayerPosition = isPlayer(grid, newPosition); 

    if (moveToNewPosition(player, newPosition, map)) {
        // update the "seen" string
        // only for regular player
        if (!player->player_isSpectator) {
            // if new position is gold, update player_amountOfGold
            updateGoldAmount(player, grid); 
            //update the player's visibility
            player_updateVisibility(player, grid); 
        } else {
            // only for spectator
            player_updateSpecVisibility(player, grid); 
            // no need to update gold amount for spectator
        }
        // update the playerArray if it's regular player or spectator 
        updatePlayerArray(player, grid); 
    }

    // switch the player, 
    // and update the playerArray of the occupied one, not the invader 
    // because we've updated the playerArray of the invader above 
    if (occupiedPlayerPosition != -1) {
        player_t* occupiedPlayer = grid->playerArray[occupiedPlayerPosition]; 
        // change the position of the occupied player
        occupiedPlayer->player_position = oldPosition; 
        // update the visibility of the occupied player 
        player_updateVisibility(occupiedPlayer, grid); 
        // update the playerArray of the occupied player
        updatePlayerArray(occupiedPlayer, grid); 
        // update the visibility of the invader
        player_updateVisibility(player, grid); 

        // steal the gold
        int goldStolen = occupiedPlayer->player_amountOfGold * steal_percentage; 
        occupiedPlayer->player_amountOfGold -= goldStolen; 
        player->player_amountOfGold += goldStolen; 
    }

    return true; 
}

// check if char is one of the following: '|', '+', '#', '-', ' '
static bool isBlockChar(char ch) {
    return ch == '|' || ch == '+' || ch == '#' || ch == '-' || ch == ' '; 
}

// update the playerArray in grid
static void updatePlayerArray(player_t* player, grid_t* grid) {
    for (int i = 0; i < max_player_number + 1; i++) {
        // find the correct player
        if (grid->playerArray[i] != NULL && message_eqAddr(grid->playerArray[i]->player_address, player->player_address)) {
            // update it's position
            grid->playerArray[i] = player; 
            break; 
        }
    }
}

// update amount of gold by current player
static void updateGoldAmount(player_t* player, grid_t* grid) {
    int amountOfGold = counters_get(grid->goldTable, player->player_position); 
    if (amountOfGold > 0) {
        // update gold amount of current player
        player->player_amountOfGold += amountOfGold; 
        // set current gold to be 0
        counters_set(grid->goldTable, player->player_position, 0); 
        grid -> gridString[player -> player_position] = available_mark;
    }
}

// check whether current position is a player
// if it's a player, return it's position in the playerArray
// otherwise, return -1 
static int isPlayer(grid_t* grid, int position) {
    // add 1 to consider spectator 
    for (int i = 0; i < max_player_number + 1; i++) {
        // need to check spectator here
        if (grid->playerArray[i] != NULL && !grid->playerArray[i]->player_isSpectator) {
            if (position == grid->playerArray[i]->player_position) {
                return i; 
            }
        }
    }
    return -1; 
}

// check whether it's the last passage
static bool isLastPassage(player_t* player, char* map, int currIndex, int width) {
    // check 4 directions
    // if there's at least 1 direction is '#' (not previously seen), return false
    // else, return true
    int leftIndex = currIndex - 1, rightIndex = currIndex + 1, 
        upIndex = currIndex - width, downIndex = currIndex + width; 

    char *leftIndexStr = intToString(leftIndex), *rightIndexStr = intToString(rightIndex), 
        *upIndexStr = intToString(upIndex), *downIndexStr = intToString(downIndex); 

    if (leftIndex >= 0 && map[leftIndex] == '#' && hashtable_find(player->player_passageVisited, leftIndexStr) == NULL) {
        free(leftIndexStr);
        free(rightIndexStr);
        free(upIndexStr);
        free(downIndexStr);
        return false; 
    } 
    if (rightIndex < strlen(map) && map[rightIndex] == '#' && hashtable_find(player->player_passageVisited, rightIndexStr) == NULL) {
        free(leftIndexStr);
        free(rightIndexStr);
        free(upIndexStr);
        free(downIndexStr);
        return false; 
    } 
    if (upIndex >= 0 && map[upIndex] == '#' && hashtable_find(player->player_passageVisited, upIndexStr) == NULL) {
        free(leftIndexStr);
        free(rightIndexStr);
        free(upIndexStr);
        free(downIndexStr);
        return false; 
    } 
    if (downIndex < strlen(map) && map[downIndex] == '#' && hashtable_find(player->player_passageVisited, downIndexStr) == NULL) {
        free(leftIndexStr);
        free(rightIndexStr);
        free(upIndexStr);
        free(downIndexStr);
        return false; 
    } 
    free(leftIndexStr);
    free(rightIndexStr);
    free(upIndexStr);
    free(downIndexStr);
    return true; 
}

// convert from int to string
static char* intToString(int a) {
    char* str = malloc(80);  
    sprintf(str, "%d", a); 
    return str; 
}

// update the visibility when the position is at the same column with player
static void updateVisibilitySameCol(player_t* player, char* map, int commonCol, int currRow, int playerRow, int width) {
    int minRow = min(currRow, playerRow), maxRow = max(currRow, playerRow); 
    int i = minRow + 1; 
    for (i = minRow + 1; i < maxRow; i++) {
        int index = convertToIndex(i, commonCol, width); 
        // if the path is not available
        if (map[index] != available_mark && map[index] != gold_mark) {
            break; 
        }
    }
    // if all dots along the path is visible, then current position is visible
    if (i == maxRow) {
        int index = convertToIndex(currRow, commonCol, width); 
        player->player_seen[index] = map[index]; 
    }
}

// update the visibility when the position is at the same row with player
static void updateVisibilitySameRow(player_t* player, char* map, int commonRow, int currCol, int playerCol, int width) {
    int minCol = min(currCol, playerCol), maxCol = max(currCol, playerCol); 
    int i = minCol + 1; 
    for (i = minCol + 1; i < maxCol; i++) {
        int index = convertToIndex(commonRow, i, width); 
        if (map[index] != available_mark && map[index] != gold_mark) {
            break; 
        }
    }
    // if all dots along the path is visible, then current position is visible
    if (i == maxCol) {
        int index = convertToIndex(commonRow, currCol, width); 
        player->player_seen[index] = map[index]; 
    }
}

// check whether current position is a room (
    // not blocked by walls during the path from it to player)
static bool isRoom(int currCol, int currRow, int playerCol, int playerRow, char* map, int width) {
    double k = (playerRow - currRow + 0.0) / (playerCol - currCol + 0.0); 

    bool vertical = checkVertical(map, playerCol, playerRow, currCol, currRow, width, k); 
    bool horizonal = checkHorizonal(map, playerCol, playerRow, currCol, currRow, width, k); 

    return vertical && horizonal; 
}

// check vertical walls existence
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
            if (isBlockChar(map[index])) {
                return false; 
            }  
        } else {
            // current row is actually a floating number, check it's neighbours
            // e.g.: (5, 5.2) -> check (5, 5) and (5, 6)
            int nei1 = floor(row), nei2 = nei1 + 1; 
            // convert from (col, row) to index
            int index1 = convertToIndex(nei1, col, width), index2 = convertToIndex(nei2, col, width); 
            if (isBlockChar(map[index1]) && isBlockChar(map[index2])) {
                return false; 
            }
        }
    }
    return true; 
}

// check horizonal walls existence
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
            if (isBlockChar(map[index])) {
                return false; 
            } 
        } else {
            int nei1 = floor(col), nei2 = nei1 + 1; 
            int index1= convertToIndex(row, nei1, width), index2 = convertToIndex(row, nei2, width); 
            if (isBlockChar(map[index1]) && isBlockChar(map[index2])) {
                return false; 
            } 
        }
    }
    return true; 
}

// convert from [row, col] to index in the map
static int convertToIndex(int row, int col, int width) {
    return row * width + col; 
}

// check whether a floating number is actually equal to an int
static bool isInteger(double a) {
    int b = a; 
    double diff = a - b; 
    return diff == 0; 
}

// return the row number of current position
static int getRow(int position, int width) {
    return position / width; 
}

// return the column number of current position
static int getCol(int position, int width) {
    return position % width;  
}

// check whether current position reaches the boundary of the map
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

// move the player
static bool moveToNewPosition(player_t*player, int newPosition, char* map) {
    // if new position is reachable, then move to it
    // otherwise, do nothing
    if (checkValidPosition(newPosition, map)) {
        // reset the originl position because we're moving
        player->player_seen[player->player_position] = map[player->player_position]; 
        player->player_position = newPosition; 
        player->player_seen[player->player_position] = player_mark; 
        return true; 
    }
    return false; 
}

// check whether current position is valid so that player can move to
static bool checkValidPosition(int position, char* map) {
    if (map[position] == available_mark || map[position] == '#' || map[position] == gold_mark) {
        return true; 
    } 
    return false; 
}
