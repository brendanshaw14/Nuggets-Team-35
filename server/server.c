/* server.c file for the Nuggets game project
Authors: Brendan Shaw, Ming Cheng, Romeo Myrthil
CS50- Winter 2023
*/

//Includes: 
#include "../support/message.h"
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//function headers:
void parseArgs(const int argc, const char* argv[], int* seed);
bool handleTimeout(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);


/********************Main******************/
/* Psuedocode:
*/
int main(const int argc, const char* argv[]){
    int seed = 0;
    parseArgs(argc, argv, &seed);
    //initialize the message stream
    message_init(stderr);
    //loop through messages
    message_loop(NULL, 100, handleTimeout, handleInput, handleMessage);
    //end the message stream
    message_done();

    return 0;
}

/*************parseArgs*************/
/*  Check that either 1 or 2 arguments were provided
    if not, print error message, exit non-zero
    if so, verify the map can be opened for reading
        if 1 argument
            srand(getpid());
        else 
            srand(argv[3]);
NOTE: THIS METHOD ACCEPTS EXTRANEOUS CHARACTERS INCLUDED IN THE SEED ARGUMENT SO LONG AS AN INT CAN BE PARSED
*/
void parseArgs(const int argc, const char* argv[], int* seed){
    //if one or two args were provided
    if (argc == 2 || argc == 3){
        //test the map is openeable for reading
        FILE* map;
        map = fopen(argv[1], "r");
        //if not opened, print error and exit non-zero
        if (map == NULL){
            fprintf(stderr, "Error: Unable to read map file input.");
            exit(2);
        }
        if (argc == 3){
            if (sscanf(argv[2], "%d", seed) != 1){
                fprintf(stderr, "Error: Unable to read seed");
                exit(3);
            }
        }
    }
    else{ 
        fprintf(stderr, "Error: Incorrect number of arguments");
        exit(1);
    }
    return;
}
//handle timeout starter function
bool handleTimeout(void* arg){
    return true;
}

//handle input starter function
bool handleInput(void* arg){
    return true;
}

//handle message starter function
bool handleMessage(void* arg, const addr_t from, const char* message){
    printf("%s\n", message);
    return true;
}

