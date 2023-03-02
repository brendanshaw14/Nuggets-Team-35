/* gridtest.c- a test program for the grid.c module.

Author: Brendan Shaw, February 2023*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grid.h"
#include "../player/player.h"


int main(const int argc, const char* argv[]){
    FILE* map;
    //load some mapos 
    printf("\nLoading the main map:");
    map = fopen("../maps/main.txt", "r");
    grid_t* grid = grid_init(map);
    printf("\nPrinting the first grid:\n %s", grid -> gridString);
    printf("\nAdding the gold with Null seedd. ");
    grid_placeGold(grid, 10, 30, 250, -1);
    printf("\nPrinting the first grid with gold: %s", grid -> gridString);
    grid_delete(grid);
    fclose(map);
    return 0;
}

