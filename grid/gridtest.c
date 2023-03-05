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

//headers- both helpers for counters iterate
void findGoldSum(void* arg, const int key, const int count);
void findNumPiles(void* arg, const int key, const int count);


int main(const int argc, const char* argv[]){
    FILE* map1;
    FILE* map2;
    FILE* map3;
    //test grid_init: make some grids
    grid_t* grid1;
    grid_t* grid2;
    grid_t* grid3;
    grid_t* grid4;
    grid_t* grid5;
    //load the main grid
    printf("\nLoading the main map:");
    map1 = fopen("../maps/main.txt", "r");
    if ((grid1 = grid_init(map1))){
        printf("\nGrid Init Success\n");
    }
    //load the big grid
    printf("\nLoading the big map:");
    map2 = fopen("../maps/big.txt", "r");
    if ((grid2 = grid_init(map2))){
        printf("\nGrid Init Success\n");
    }
    //load the hole grid
    printf("\nLoading the hole map:");
    map3 = fopen("../maps/hole.txt", "r");
    if ((grid3 = grid_init(map3))){
        printf("\nGrid Init Success\n");
    }
    //print all
    printf("\nPrinting the \"main\" grid:\n%s", grid1 -> gridString);
    printf("\nPrinting the \"big\" grid:\n%s", grid2 -> gridString);
    printf("\nPrinting the \"hole\" grid:\n%s", grid3 -> gridString);
    
    //test grid_placeGold
    printf("\nAdding the gold with random seed to all maps. ");
    grid_placeGold(grid1, 10, 30, 250, -1);
    grid_placeGold(grid2, 10, 30, 250, -1);
    grid_placeGold(grid3, 10, 30, 250, -1);
    //print the first counters to show gold distribution
    printf("\nPrinting gold counters to prove correct distribution:\n");
    counters_print(grid1->goldTable, stdout);
    int sumGold = 0;
    int numPiles = 0;
    counters_iterate(grid1 -> goldTable, &sumGold, findGoldSum);
    counters_iterate(grid1 -> goldTable, &numPiles, findNumPiles);
    printf("\n Printing total gold added to the counters: %d", sumGold);
    printf("\n Printing the number of piles added: %d", numPiles);
    printf("\nPrinting the first grid with gold:\n%s", grid1 -> gridString);
    //print the second counters to show gold distribution
    printf("\nPrinting gold counters to prove correct distribution:\n");
    counters_print(grid2->goldTable, stdout);
    sumGold = 0;
    numPiles = 0;
    counters_iterate(grid2 -> goldTable, &sumGold, findGoldSum);
    counters_iterate(grid2 -> goldTable, &numPiles, findNumPiles);
    printf("\n Printing total gold added to the counters: %d", sumGold);
    printf("\n Printing the number of piles added: %d", numPiles);
    printf("\nPrinting the second grid with gold:\n%s", grid2 -> gridString);
    //print the third counters to show gold distribution
    printf("\nPrinting gold counters to prove correct distribution:\n");
    counters_print(grid3->goldTable, stdout);
    sumGold = 0;
    numPiles = 0;
    counters_iterate(grid3 -> goldTable, &sumGold, findGoldSum);
    counters_iterate(grid3 -> goldTable, &numPiles, findNumPiles);
    printf("\n Printing total gold added to the counters: %d", sumGold);
    printf("\n Printing the number of piles added: %d", numPiles);
    printf("\nPrinting the first grid with gold:\n%s", grid3 -> gridString);

    //showing that using seed -1 (random seed) will always be different:
    printf("\n\nshowing that using set seed will always be the same:\n");
    grid4 = grid_init(map1);
    grid5 = grid_init(map1);
    grid_placeGold(grid4, 10, 30, 250, 1);
    grid_placeGold(grid5, 10, 30, 250, 1);
    printf("\nPrinting the main map with seed 1 gold:\n%s", grid4 -> gridString);
    printf("\nPrinting a new grid with the same map and seed: should be the same:\n%s", grid5 -> gridString);

    //testing grid_addPlayer
    addr_t* address1 = malloc(sizeof(addr_t));
    addr_t* address2 = malloc(sizeof(addr_t));
    player_t* player1 = player_init(grid1, *address1, "player1", false, 5, 'A');
    player_t* player2 = player_init(grid1, *address2, "player2", false, 5, 'B');
    grid_addPlayer(grid1, player1);
    grid_addPlayer(grid1, player2);

    //test delete and cleanup: SEE MAKE VALGRIND
    fclose(map1);
    fclose(map2);
    fclose(map3);
    free(address1);
    free(address2);
    grid_delete(grid1);
    grid_delete(grid2);
    grid_delete(grid3);
    grid_delete(grid4);
    grid_delete(grid5);
    return 0;
}

// finds the sum of all gold values placed into the gold counters
void findGoldSum(void* arg, const int key, const int count){
    int* sum = arg;
    *sum += count;
    return;
}

// finds the total number of piles placed
void findNumPiles(void* arg, const int key, const int count){
    int* numPiles = arg;
    *numPiles += 1;
    return;
}




