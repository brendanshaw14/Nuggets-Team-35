/* gridtest.c- a test program for the grid.c module.

Author: Brendan Shaw, February 2023*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grid.h"


int main(const int argc, const char* argv[]){
    //verify arg file can be opened for reading
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "Invalid Grid Test Map");
        exit(1);
    }
    grid_t* grid = grid_init(fp);
    printf("%s", grid -> gridString);
    grid_placeGold(grid, 10, 30, -1);
    printf("%s", grid -> gridString);
    printf("player: s")
    return 0;
}

