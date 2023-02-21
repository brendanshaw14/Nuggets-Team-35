/* server.c file for the Nuggets game project
Authors: Brendan Shaw, Ming Cheng, Romeo Myrthil
CS50- Winter 2023
*/

//Includes: 
#include "support/message.h"


//function headers:
bool handleTimeout(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);



/********************Main******************/
/* Psuedocode:
*/
int main(const int argc, const char* argv[]){
    //initialize the message stream
    message_init(stderr);
    //loop through messages
    message_loop(NULL, 100, handleTimeout, handleInput, handleMessage);
    //end the message stream
    message_done();

    return 0;
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
    return true;
}

