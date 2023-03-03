/* gridtest.c- a test program for the grid.c module.

Author: Brendan Shaw, February 2023*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grid.h"
#include "../player/player.h"
#include "../support/message.h"

//headers
void findGoldSum(void* arg, const int key, const int count);


int main(const int argc, const char* argv[]){
    FILE* map;
    //test grid_init
    printf("\nLoading the main map:");
    map = fopen("../maps/main.txt", "r");
    grid_t* grid = grid_init(map);
    printf("\nPrinting the \"main\" grid:\n%s", grid -> gridString);
    
    //test grid_placeGold
    printf("\nAdding the gold with random seed. ");
    grid_placeGold(grid, 10, 30, 250, -1);
    printf("\nPrinting gold counters:\n");
    counters_print(grid->goldTable, stdout);
    int sumGold = 0;
    counters_iterate(grid -> goldTable, &sumGold, findGoldSum);
    printf("\n Printing total gold added to the counters: %d", sumGold);
    printf("\nPrinting the first grid with gold:\n%s", grid -> gridString);

    //testing grid_addPlayer
    addr_t* address1 = malloc(sizeof(addr_t));
    player_t* player1 = player_init(grid, *address1, "player1", false, 5, 'A');
    grid_addPlayer(grid, player1);
    grid_delete(grid);
    fclose(map);
    return 0;
}

// finds the sum of all gold values placed into the gold counters
void findGoldSum(void* arg, const int key, const int count){
    int* sum = arg;
    *sum += count;
    return;
}



