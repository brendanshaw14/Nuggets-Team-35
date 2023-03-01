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
    //invalid map tests
    printf("\nInvalid args tests:\n");
    printf("No args:\n");
    grid_t* grid = grid_init("main.txt"); 
    return 0;
}

