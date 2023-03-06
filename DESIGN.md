# CS50 Nuggets
## Design Spec
### Team 35, Winter, 2023

Team Members: Ming Cheng, Romeo Myrthil, Brendan Shaw


According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes a `Player` and a `Grid` modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

### User interface

Because we have only three group members, we will be using the `client` executable given to us in the `shared` library instead of writing out own, which takes two three arguments: 

 > Example: ./client hostname port [playername] 

See the requirements spec for both the command-line and interactive UI.

### Inputs and outputs- Keystrokes and Display

Pulled from requirements spec for reference: 

* `Q` : quit- turns off display and exits the client. 
* `h` : move left- moves the player's location and icon to the left, if possible.  
* `l` : move right- moves the player's location and icon to the right, if possible.  
* `j` : move down- moves the player's location and icon down, if possible.  
* `k` : move up- moves the player's location and icon up, if possible.  
* `y` : move diagonally up and left- moves the player's location and icon diagonally and to the left, if possible.  
* `u` : move diagonally up and right- moves the player's location and icon diagonally and to the right, if possible.  
* `b` : move diagonally down and left- moves the player's location and icon diagonally and to the left, if possible.  
* `n` : move diagonally down and left- moves the player's location and icon diagonally and to the left, if possible.  

### Functional decomposition into modules
See above: client was given to us.

### Pseudo code for logic/algorithmic flow
See above: client was given to us.

### Major data structures
See above: client was given to us.

---

## Server
### User interface

The `server` module will be run through the command line given two arguments: the `map.txt` file and the gold random generation seed (optional). 
 > Example: ./server map.txt [seed]

See the requirements spec for the command-line interface.
There is no interaction with the user.

### Inputs and outputs

The `server` takes a `.txt` file containing a _valid_ nuggets map specified by the user. This map will then be loaded by the grid module for use by the server and client.

We anticipate only the server port and error messages to be written to stdout. 

We anticipate all messages received and sent by the server will be written to a log file. 

### Functional decomposition into modules

We anticipate three main methods will be used in the server module to handle arguments, starting the game, and ending the game:
* `parseArgs`- handles and verifies arguments
* `initializeGame`- configures the `Game` struct and loads the grid
* `gameOver`- informs all clients that the game has ended

Three other helper functions will also be written within server to pass to the `message_loop` method, in order to handle timeout and the messages themselves. 

* `handleTimeout`- ends the message loop after a certain amount of time.
* `handleStdout`- handles input from stdin.
* `handleMessage`- processes messages from the client and handles them accordingly.


We also anticipate the use of two helper modules for our server, `Grid` and `Player`. 
* `Grid`- stores the current map itself and any other necessary information, such as the map parameters and all players information in the game. 
* `Player`- represents a client in the game and is used by the server and grid for easy access to their information. 

### Pseudo code for logic/algorithmic flow

The server will run as follows:

    execute from a command line per the requirement spec
    parse the command line, validate parameters
    call initializeGame() to set up data structures 
        create a hashtable of players
        load the map into a grid
        distribute gold randomly into the map
    call message_init to initialize the 'message' module
    print the port number on which we wait
    call message_loop(), to await clients, passing it our helper functions, and wait for messages
    for each message: 
        call message handle (see below) 
    call gameOver() to inform all clients the game has ended
    clean up


#### main


    execute from a command line per the requirement spec
    parse the command line, validate parameters
    call initializeGame() to set up data structures
    initialize the 'message' module
    print the port number on which we wait
    call message_loop(), to await clients, passing it our helper functions
    call gameOver() to inform all clients the game has ended
    clean up

#### parseArgs


    ensure that the mapFile is readable
    if there is a seed, save it

#### initializeGame


    load the grid from the mapFile, create a string to represent the grid
    create the player hashtable 
    initialize the player struct

#### gameOver


    send the quit message to all clients, along with the reason

#### handleTimeout


    if the given amount of time passes without a message, close the server

#### handleMessage


    if the message is PLAY
        create a new Player struct, initialize its values, and add it to the hashtable
        make sure the player can join and that their name is valid
        set the player's position randomly and add the player to the grid 
        store their name, send the OK <L> message to the client
        send the GRID message to send nrows and ncolumns
    if the message is SPECTATE
        if there is already a spectator
            remove the current spectator
            create a new Player struct, initialize its values, add it to the hashtable
            store their name, send the message to the client
        change the players mode to spectate
        send them the whole display
    if the message is KEY
        if the message is from a player
            if it is a move key
                move the player (will use helper functions)
                send each client their display (with visibility)
                if the player got gold too
                    update the amount of gold that this player has found
                    if that is the max amount of gold, quit the game
                    otherwise send the GOLD message to all clients (including updated numbers for spectators)
            if it is an invalid key
                send an error message, ignore the keystroke
        if the message is from a spectator
            make sure it's q 
            quit 
    if all the gold was collected, quit the game
    send the updated display message to all clients


### Major data structures

#### Hashtable 

A data structure `hashtable` will be used to store the locations and amounts of gold piles across the map. The key would in this case be an index in the map string, which would be a spot on the map, and the item would an amount of gold stored as an int.

The `hashtable` structure will also needed to store the players information to get access to all players in the `Grid` module. It contains the items with type of `player_t` which stores the player's information.

---
## Player

The player module aims to collect and manage the information of all players. 

It mainly contains the following:
```c
struct Player {
    int player_address;
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen;  
}; 
```

1. `player_address` is the player's client address.

2. `player_position` is the index of the grid string to represent the position of current player. For example: `grid_string[index]`.

3. `player_name` is the string indicating the player's name.

4. `player_amountOfGold` is an integer indicating the amount of gold found by current player. Every time a player finds a gold, we should update its value.

5. `player_seen` is a string indicating the positions that this player has aleady seen. It should have the same length as the total map.

The functions it contains are listed below.

### Functional decomposition

#### player_getName

The definition of this function is `char* player_getName();`. It returns the name of the current player. 

#### player_getPosition

The definition of this function is `int player_getPosition();`. It returns an integer, as an index of the string, to represent the position of current player. 

#### player_getGold

The definition of this function is `int player_getGold();`. It returns the amount of golds found by the current player. 

#### player_move

The definition of this function is `bool player_move(char k);`. It returns `true` when the input character `k` is valid, and vice versa.

#### player_getVisibility

The definition of this function is `void player_getVisibility(char* player_seen, int position)`. It modifies the `player_seen` string.   


### Pseudo code for logic/algorithmic flow

#### player_getName
    return Player->player_name; 

#### player_getPosition
    return Player->player_position; 

#### player_getGold
    return Player->player_amountOfGold; 

#### player_move
    if input char is not valid:
        return false; 
    else, parse the char:
        if new position is not accessable (e.g. wall, spaces):
            return true; 
        else: 
            update the player's position
            return true; 




#### player_getVisibility

```c
    initialize the "player_seen" string with empty spaces
    loop for all chars in the string: 
        if the distance between current char and the player is smaller than the visibility range:
            update current char
```


### Major data structures

We only need a string to represent player's name and an integer for player's position, there's no data structure in this module. 

## Grid

The grid module ultimately stores and maintains the game map itself, containing methods allowing for easy access and updates to information in the map.  

It mainly contains the following:
```c
struct Grid {
    char* grid;
    Int width;
    Int height;
    Int numPlayers;
    hashtable_t* players; 
    int goldTotalLeft; 
    hashtable_t* goldLocation;
}; 
```

### Functional decomposition

* `grid_init` - reads a map into a string and returns a grid_t structure with the string, and the width and height of the map.
* `grid_placeGold` - takes the grid and the gold constants and returns a hashtable of gold (where the index to a point in the string is the key, and the amount of gold is the item).
* `grid_getGoldLeft` - return the amount of gold left to claim on the map.

### Pseudo code for logic/algorithmic flow

#### grid_init
```c
    loop through lines in map.txt
      add lines to char*
      for each line that is read, add +1 to height
    get width by dividing length of string by height
```

#### grid_placeGold
```c
    initialize hashtable
    loop while goldplaced != goldTotal
      get a random index in grid string
      check if index is valid position
        get a random int newGoldPile between goldMin and goldMax
        if goldplaced + newGoldPile < goldTotal
          insert in hashtable
```

#### grid_getGoldLeft
```c
    return gridStruct->goldTotalLeft
```

## Testing Plan

Current testing plan is not in detailed, but in a high-level way.

1. Test when the number of players exceeds the maximum range. 
2. Test when the length of player's name exceeds the maximum range.
3. Test when the input key is invalid.
4. Test when the player is reaching the boundary of the map.
5. Test when the player is moving (toward all of the 8 directions).
6. Test when the total amount of gold found reaches the maximum.
7. Test when the user quits the game.
8. Test the visibility display of each player and the spectator.
