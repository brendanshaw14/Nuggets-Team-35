/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player.h for detailed info.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include "../libcs50/file.h"
#include "player.h"


char* loadMap(FILE* inputMap, int* widthPtr, int* heightPtr) {
    //initialize the new grid structs
    //get row and column parameters
    int numRows = file_numLines(inputMap); //set the number of rows
    char* firstLine = file_readLine(inputMap); 
    int numColumns = strlen(firstLine);

    *widthPtr = numColumns; 
    *heightPtr = numRows; 

    char* map = malloc((numColumns + 1) * numRows + 1); 
    char* nextLine = malloc(numColumns + 1);
    //put the first line in    
    strcat(firstLine, "\n");
    strcat(map, firstLine);
    //read through the lines of the map, adding the null terminator and pasting them into the gridString
    while ((nextLine = file_readLine(inputMap)) != NULL){
        strcat(nextLine, "\n");
        strcat(map, nextLine);
    }
    return map;
}

int main(const int argc, const char* argv[]) {
    FILE* fp = fopen(argv[1], "r"); 
    int width = 0, height = 0; 
    char* map = loadMap(fp, &width, &height); 
    printf("%s", map);

    printf("width: %d, height: %d\n", width, height); 

    // create a player
    player_t* player = player_init(map, -1, 1064, "", false); 

    char ch = '\0'; 
    int radius = 18; 
    // set the init position of player
    printf("position: %d\n", player->player_position); 
    // mark the player, show init visible range 
    player_updateVisibility(player, map, width, height, radius); 
    printf("%s\n\n", player->player_seen); 
    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n')
            continue; 
        printf("curr direc: %c\n", ch); 
        player_move(player, (char)ch, width, height, map, radius); 
        printf("%s\n\n", player->player_seen); 
    }

    return 0;
}