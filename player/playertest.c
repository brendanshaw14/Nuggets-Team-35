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
#include "../grid/grid.h"

int main(const int argc, const char* argv[]) {
    FILE* fp = fopen(argv[1], "r"); 
    int width = 0, height = 0; 

    printf("width: %d, height: %d\n", width, height); 

    // create a player
    int radius = 18; 
    grid_t* grid = grid_init(fp); 
    player_t* player = player_init(grid, 100, "1", false, radius); 
    // player_t* player1 = player_init(grid, -1, 770, "", false, radius); 
    player_t* player1 = player_init(grid, 101, "2", true, radius);

    // add these two players into grid (manually for now)
    grid_addPlayer(grid, player); 
    grid_addPlayer(grid, player1); 

    // visualize the two players initially 
    player_updateVisibility(player, grid); 
    player_updateVisibility(player1, grid); 

    printf("player1: \n%s\n", player->player_seen); 
    printf("player2: \n%s\n", player1->player_seen); 

    char ch = '\0'; 
    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n')
            continue; 
        printf("curr direc: %c\n", ch); 
        player_move(player, grid, ch); 
        printf("current regular player\n %s\n\n", player->player_seen); 
        // refresh what spectator sees 
        player_updateSpecVisibility(player1, grid); 
        printf("current spec player \n %s\n\n", player1->player_seen); 
    }

    return 0;
}