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
#include "../support/message.h"

int main(const int argc, const char* argv[]) {
    FILE* fp = fopen(argv[1], "r"); 
    int width = 0, height = 0; 

    printf("width: %d, height: %d\n", width, height); 

    // create a player
    int radius = 18; 
    grid_t* grid = grid_init(fp); 
    addr_t* address1 = malloc(sizeof(addr_t)); 
    addr_t* address2 = malloc(sizeof(addr_t)); 
    addr_t* address_spec = malloc(sizeof(addr_t)); 

    // set these addresses to be different manually
    address1->sin_family = -1; 
    address2->sin_family = 0; 
    address_spec->sin_family = 1; 

    player_t* player = player_init(grid, *address1, "name1", false, radius, 'A'); 
    player_t* player1 = player_init(grid, *address2, "name2", false, radius, 'B');
    player_t* spectator = player_init(grid, *address_spec, "name_spec", true, radius, ' ');


    // add these two players into grid (manually for now)
    grid_addPlayer(grid, player); 
    grid_addPlayer(grid, player1); 
    grid_addPlayer(grid, spectator); 

    // place the gold
    grid_placeGold(grid, 10, 30, 250, -1);

    // visualize the two players initially 
    player_updateVisibility(player, grid); 
    player_updateVisibility(player1, grid); 
    player_updateSpecVisibility(spectator, grid); 


    printf("player1: \n%s\n", player->player_seen); 
    printf("player2: \n%s\n", player1->player_seen); 
    printf("spec: \n%s\n", spectator->player_seen); 


    char ch = '\0'; 
    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n')
            continue; 
        printf("curr direc: %c\n", ch); 
        player_move(player1, grid, ch); 
        printf("current regular player\n%s\n\n", player1->player_seen); 
        // refresh what spectator sees 
        player_updateSpecVisibility(spectator, grid); 
        printf("current spec player \n%s\n\n", spectator->player_seen); 
    }
    grid_delete(grid);

    return 0;
}