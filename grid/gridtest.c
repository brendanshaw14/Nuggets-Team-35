/* gridtest.c- a test program for the grid.c module.

Author: Brendan Shaw, February 2023*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grid.h"
#include "../player/player.h"

//headers
void findGoldSum(void* arg, const int key, const int count);


int main(const int argc, const char* argv[]){
    FILE* map;
    //load some maps 
    printf("\nLoading the main map:");
    map = fopen("../maps/main.txt", "r");
    grid_t* grid = grid_init(map);
    printf("\nPrinting the \"main\" grid:\n%s", grid -> gridString);
    printf("\nAdding the gold with random seed. ");
    grid_placeGold(grid, 10, 30, 250, -1);
    printf("\nPrinting gold counters\n");
    counters_print(grid->goldTable, stdout);
    int sumGold = 0;
    counters_iterate(grid -> goldTable, &sumGold, findGoldSum);
    printf("\n Printing total gold added to the counters: %d", sumGold);
    printf("\nPrinting the first grid with gold:\n%s", grid -> gridString);
    grid_delete(grid);
    fclose(map);
    return 0;
}

void findGoldSum(void* arg, const int key, const int count){
    int* sum = arg;
    *sum += count;
    return;
}