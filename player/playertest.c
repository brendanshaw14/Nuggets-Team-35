/* Player.c - the Player module for the CS50 nuggets project
Authors: Brendan Shaw, Romeo Myrthil, Ming Cheng
CS50- Winter 2023
See player1.h for detailed info.*/

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
    // define radius 
    int radius = 5; 
    grid_t* grid = grid_init(fp);
    // define address for player 1
    addr_t* address1 = malloc(sizeof(addr_t)); 
    // define address for player 2
    addr_t* address2 = malloc(sizeof(addr_t)); 
    // define address for spectator
    addr_t* address_spec = malloc(sizeof(addr_t)); 

    // set address to be different manually for now
    address1->sin_family = 0; 
    address2->sin_family = 1; 
    address_spec->sin_family = 2; 

    // create player 1
    player_t* player1 = player_init(grid, *address1, "name1", false, radius, 'A'); 
    // create player 2
    player_t* player2 = player_init(grid, *address2, "name2", false, radius, 'B');
    // create spectator
    player_t* spectator = player_init(grid, *address_spec, "name_spec", true, radius, ' ');

    // test if player is created correctly
    // for player 1
    printf("player1 position (index, should be 0): %d\n", player1->player_position); 
    printf("player1 name (should be name1): %s\n", player1->player_name); 
    printf("player1 amount of gold (should be 0): %d\n", player1->player_amountOfGold); 
    printf("player1 visibility range (should be %d): %d\n", radius, player1->player_visibility_range);
    printf("player1 seen string (should be empty lines): %s\n", player1->player_seen); 
    printf("player1 letter (should be 'A'): %c\n", player1->player_letter);
    printf("player1 activate status (should be true, print 1 here): %d\n", player1->player_isActivate);

    // for player 2
    printf("player2 position (index, should be 0): %d\n", player2->player_position); 
    printf("player2 name (should be name2): %s\n", player2->player_name); 
    printf("player2 amount of gold (should be 0): %d\n", player2->player_amountOfGold); 
    printf("player2 visibility range (should be %d): %d\n", radius, player2->player_visibility_range);
    printf("player2 seen string (should be empty lines): %s\n", player2->player_seen); 
    printf("player2 letter (should be 'B'): %c\n", player2->player_letter);
    printf("player2 activate status (should be true, print 1 here): %d\n", player2->player_isActivate);

    // for spectator
    printf("spectator position (index, should be 0): %d\n", spectator->player_position); 
    printf("spectator name (should be name_spec): %s\n", spectator->player_name); 
    printf("spectator amount of gold (should be 0): %d\n", spectator->player_amountOfGold); 
    printf("spectator visibility range (should be %d): %d\n", radius, spectator->player_visibility_range);
    printf("spectator seen string (should be the map): \n%s\n", spectator->player_seen); 
    printf("spectator letter (should be ' '): %c\n", spectator->player_letter);
    printf("spectator activate status (should be true, print 1 here): %d\n", spectator->player_isActivate);

    // add these two players into grid (manually for now)
    grid_addPlayer(grid, player1); 
    grid_addPlayer(grid, player2); 
    grid_addPlayer(grid, spectator); 

    // test player's position, should not be 0
    printf("player1 position (index, should not be 0): %d\n", player1->player_position); 
    printf("player2 position (index, should not be 0): %d\n", player2->player_position); 
    printf("spectator position (index, should not be 0): %d\n", spectator->player_position); 


    // add gold
    // "grid_placeGold" function is already tested in gridtest
    grid_placeGold(grid, 10, 30, 250, -1); 

    // visualize the two players 
    player_updateVisibility(player1, grid); 
    player_updateVisibility(player2, grid); 
    // visualize the spectator 
    player_updateSpecVisibility(spectator, grid); 

    // test player initial visibility
    printf("player1 initial visibility: \n%s\n", player1->player_seen); 
    printf("player2 initial visibility: \n%s\n", player2->player_seen); 
    printf("spectator initial visibility: \n%s\n", spectator->player_seen); 

    // test when there's inactivate player, the spectator will not see it
    // set player1 to be inactivate
    player1->player_isActivate = false; 
    // refresh what spectator sees 
    player_updateSpecVisibility(spectator, grid); 
    printf("spectator visibility (should not see player A): \n%s\n", spectator->player_seen);

    // after testing, change everything back
    player1->player_isActivate = true; 
    // refresh what spectator sees 
    player_updateSpecVisibility(spectator, grid); 

    char ch = '\0'; 
    bool ret = false; 

    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n') 
            continue; 
        if (ch == 't') {
            while ((ch = fgetc(stdin)) != EOF) {
                if (ch == 't')
                    break;
                printf("curr direction: %c\n", ch); 
                ret = player_move(player1, grid, ch); 
                if (!ret) {
                    fprintf(stderr, "player_move function failed.\n"); 
                }
                printf("current player1 visibility: \n%s\n\n", player1->player_seen); 
                // refresh what spectator sees 
                player_updateSpecVisibility(spectator, grid); 
                printf("current spectator visibility: \n%s\n\n", spectator->player_seen);
                printf("current amount of gold of player1: %d\n", player1->player_amountOfGold); 
                printf("current amount of gold of player2: %d\n", player2->player_amountOfGold); 
            } 
        }
        printf("curr direction: %c\n", ch); 
        ret = player_move(player2, grid, ch); 
        if (!ret) {
            fprintf(stderr, "player_move function failed.\n"); 
        }
        printf("current player2 visibility: \n%s\n\n", player2->player_seen); 
        // refresh what spectator sees 
        player_updateSpecVisibility(spectator, grid); 
        printf("current spectator visibility: \n%s\n\n", spectator->player_seen);
        printf("current amount of gold of player1: %d\n", player1->player_amountOfGold); 
        printf("current amount of gold of player2: %d\n", player2->player_amountOfGold); 
    }

    // delete the player address
    free(address1); 
    free(address2); 
    free(address_spec); 
    // delete grid and all players 
    grid_delete(grid);
}