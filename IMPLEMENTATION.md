# CS50 Nuggets
## Implementation Spec
### Team 35, Winter, 2023

Team Members: Ming Cheng, Romeo Myrthil, Brendan Shaw

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes the `player` and `grid` modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

Since we only have to build the server component of the project, our main focuses are on the `grid` and `player` modules as well as the helper functions (such as `handleMessage`) for the `messageLoop` function. 

As of right now, Brendan has focused primarily on the `grid` module, Ming has focused primarily on the `player` module, and Romeo has focused primarily on the `handleMessage` function. 

As of now, we have been doing unit testing on our own as we right each module before any pull requests, and we plan to work on system testing together once we complete our main modules. 

## Server

The server interacts with the client by allowing clients to join, creating structures to represent their data, and interacting with them and the map via messages. 

### Definition of function prototypes


#### `initializeGame`

A funciton to inialize the grid module and the game structure's members. 

```c
grid_t* initializeGame(const int argc, const char* argv[]);
```

#### `handleMessage`

A function to handle message from the client.

```c
bool handleMessage(void* arg, const addr_t from, const char* message); 
```

#### `handleTimeout`

A function to handle timeout.

```c
bool handleTimeout(void* arg); 
```

#### `main`

Main function.

```c
int main(const int argc, const char* argv[]); 
```

### Detailed pseudo code

#### `initializeGame`

The function `initializeGame` takes the arguments validated by parseArgs to fully initialize the game structure within the server, as well as the grid module. 

	check if input args are valid, 
	initialize the grid 
		if seed is provided, 
			place the gold by calling `grid_placeGold` function
		else, 
			place the gold by calling `grid_placeGold` function with seed as -1 

#### `message_loop`

The function `message_loop` given to us by the `message.c` file in the support folder, provides us with the ability to wait for messages sent to the server by the client. The function takes 3 helper functions as arguments and the implementation of these will be described below.

##### `handleTimeout`

After a desired amount of time `const float timeout` without a message, `message_loop` calls `handleTimeout` which runs as follows:

	print timeout message to server
	iterate thorugh players in hashtable using endGame_helper 
	close the server 
	return true 

##### `handleInput`

This function is for the client's use; we do not anticipate use.

##### `handleMessage`

Handles the different kinds of messages sent by clients and runs as follows:

	if message is PLAY
	  player_init specified in player module, intializing variables of struct
	  if numPlayers == maxPlayers
	  	message_send to client saying server is full
		return false to continue loop
	  add player to players hashtable
	  increment numPlayers and assign corresponding letter
	  message_send OK letter, where letter is the assigned letter
	  message_send GRID nrows ncols to inform player of map size 
	  update player visibility
	  message_send DISPLAY\nplayer->player_seen
	if message is SPECTATE
	  if there is another spectator 
	  	message_send QUIT to that player/spectator
		remove from hashtable
		initialize a new player struct, using spectator_init                   
		message_send GRID with full grid to spectator
	if message is KEY
	  find player in player hashtable using adress as key
	  if player is a spectator
	  	if char == 'Q'
	  	  message_send QUIT to the spectator
		  remove spectator from players hashtable
		  return false to continue loop
		else
		  message_send ERROR invalid keystroke
		  return false to continue loop
	  if player_move is successful with inputed keystroke
	  	update grid->gridString
		iterate thorugh hashtable and update players visibility using updateVisibilities_helper
		if player_foundGold update players purse and the amount of gold left  
		  iterate through hashtable and send_message too all clients about their gold using updateGoldStatus_helper     
	  else
	    message_send ERROR, invalid keystroke
	if goldRemaining == 0
	  iterate through all players and message_send QUIT using endGame_helper
	  return true to end loop

## Player

We implement a `Player` module to store the information of player/client. 

### Data structures

We define a `Player` structure to represent a player. 
```c
typedef struct player {
    addr_t player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    int player_visibility_range; 
    char* player_seen; 
    hashtable_t* player_passageVisited; 
    bool player_isSpectator; 
    char player_letter;
    bool player_isActivate; 
} player_t; 
```

Currently, a `player` structure contains the following: 
- `player_address`: a structure with type of `addr_t` representing player's address
- `player_position`: an integer representing player's position
- `player_name`: a string representing player's name
- `player_amountOfGold`: an integer representing the amount of gold that current player has found
- `player_visibility_range`: an integer representing how much a player sees
- `player_seen`: a string representing the player's current visibility (what it already saw)
- `player_passageVisited`: a hashtable storing the passages that a player has visited. We use it to check whether the current passage is the last one because the player will have different range of visibility when it reaches the last passage.
- `player_isSpectator`: a boolean variable representing whether a player is a spectator or not.
- `player_letter`: a char representing a player's letter, from 'A' to 'Z'.
- `player_isActivate`: a boolean variable representing whether current player is activate or not.

### Definition of function prototypes

```c
player_t* player_init(grid_t* grid, addr_t address, const char* name, bool isSpectator, int radius, char letter); 
```

`player_init` function is used to initialize a player. 

```c
char* player_getName(player_t* player); 
```

`player_getName` function is used to get the name of the player.

```c
int player_getPosition(player_t* player); 
```

`player_getPosition` function is used to get the position of the player. 

```c
int player_getGold(player_t* player); 
```

`player_getGold` function is used to get the amount of gold found by the player. 

```c
char* player_getVisibility(player_t* player); 
```

`player_getVisibility` function is used to print the current visible map of the player.

```c
void player_updateVisibility(player_t* player, grid_t* grid);
```
`player_updateVisibility` function is used to update the visible range (what this player sees) when player moves.


```c
bool player_move(player_t* player, grid_t* grid, char k); 
```

`player_move` function is used to move the player (update the player's position), and calls `player_updateVisibility` function to update the visible range.


### Detailed pseudo code

#### `player_init`:

Initializes a player structure, setting default values, e.g.: the players name, address, radius, letter, spectator status, and other information.

	allocate space for the structure
	set player address
	set initial player position (a place-holder here, we'll allocate the actual position in `grid_addPlayer` in grid module)
	set player name
	define an intital hashtable storing the passages that a player has visited
	set the spectator status (whether it's a player)
	set the initial amount of gold of this player
	set the visibility range of this player
	set the player letter
	set the player to be activate 
	set the initial value of "player_seen" string

#### `player_updateVisibility`

Retrieves the player's current visibility screen. 

	get the current position of player
	if player is in the passage, check if it's the last passage:
		if so, 
			do nothing
		else:
			set the player visibility range to be 1 (because the player can only see the next position when it's in the middle passage)
	loop for all positions in the map:
		for each position 'p', if it's within the range:
			if 'p' is current player,
				show the '@' for this player
			else if 'p' is in the same row as the player, 
				if all the positions between 'p' and this player are valid, 
					set 'p' to be visible
			else if 'p' is in the same column as the player, 
				if all the positions between 'p' and this player are valid, 
					set 'p' to be visible 
			else (it's in diag direction):
				for each reachable position (call "isRoom" to check):
					"isRoom" function does: 
						computes the slope of line and calls functions below to check intersection
						calls "checkVertical" to check if there`s any vertical wall
						calls "checkHorizonal" to check if there`s any horizonal wall
						current position is visible if there`s no both two kinds of walls 
					update visibility
					show other player if exists
	loop all positions in the map:
		if there is a player out of visibility range, 
			reset it (cannot show player letter)


#### `player_move`

Moves the player on the grid given the input key.

	as for input key: 
		check whether current position reaches bound
		if so, we cannot move to the new position
			directly return true
		else：
			move to new position:
				check if new position is able to move to
				if so: 
					update the player's position
					update the player's "seen" string
					return true
				else:
					return false
			if `moveToNewPosition` returns `true`: 
				check if it's a spectator:
					if so, 
						update spectator's visibility 
					else:
						update player's visibility
						update the amount of gold of this player
				update the playerArray because we change the player's position
		if two players are next to each other (we need to switch the two player):
			change the position of the occupied player
			update the visibility of the occupied player 
			update the playerArray of the occupied player
			update the visibility of the invader
			steal the gold
		return true
	else:
		return false

#### `isReachBound`

Helper function to ensure that the player can move freely. 

	check input char to distinguish directions
	if 't', checking whether it's top bound:
		check `position / width == 0`
	if 'b', checking whether it's bottom bound:
		check `position / width == height - 1`
	if 'l', checking whether it's left bound:
		check `position % width == 0`
	if 'r', checking whether it's right bound:
		check `position % width == width - 1`

#### `moveToNewPosition`

Move the player to new position. Update the `player_seen` string and `player_position`.

	check whether current position is valid
	if so: 
		reset the `player_seen`
		update the `player_position`
		return true
	else:
		return false 

#### `checkValidPosition`
Makes sure that the player is in a valid position. 

	if current position is '.' or '#' or '*':
		return true
	else:
		return false

#### `isPlayer`
Check whether there's a player in that position

	loop all entries in the `playerArray`:
		compare the position to check if there's a player in this position
		if so,
			return the entry of `playerArray`
		else, 
			return -1

#### `isLastPassage`
Check whether the passage is the last one. Because the player will have different range of visibility when it reaches the last passage (It should see more).

	check all four directions, 
	if there's at least 1 direction is '#' (and it's not previously seen), 
		return false
	else, 
		return true

#### `isRoom`
Check whether current position is a "room", which is not blocked by walls during the path from it to player.

	computes the slope of line and calls functions below to check intersection
	calls "checkVertical" to check if there`s any vertical wall
	calls "checkHorizonal" to check if there`s any horizonal wall
	current position is visible if there`s no both two kinds of walls 

#### `checkVertical`
Check vertical walls existence

	loop all columns between current position and the player, 
		compute the row number (intersection between the line and the grid)
		if the row number is an int, 
			check whether that row is valid
		else, 
			use `floor` function to check two neighbours
			check whether two neighbours are valid

#### `checkHorizonal`
Check horizonal walls existence

	loop all rows between current position and the player, 
		compute the column number (intersection between the line and the grid)
		if the column number is an int, 
			check whether that column is valid
		else, 
			use `floor` function to check two neighbours
			check whether two neighbours are valid

#### `checkPlayerLetter` 
Check whether current char is a player letter

	return true if it is between A -> Z or it's a spectator

---
## Grid

The grid module holds most of the important data for the game, including the map, a `hashtable` of `player` structures, a `counters` representing the gold piles on the map. 

```c
typedef struct grid {
    char* gridString;
    player_t* playerArray[27]; 
    counters_t* goldTable;
    int numRows;
    int numColumns;
    int numPlayers;
    int goldRemaining;
} grid_t;
```

Currently, a `grid` structure contains the following: 
- `gridString`: a string representing the entire map
- `playerArray`: an array to store all players 
- `goldTable`: a counters object that holds the indexes of gold piles and how much gold is in that pile
- `numRows`: an integer representing the number of rows in the map
- `numColumns`: an integer representing the number of columns in the map
- `numPlayers`: an integer representing the number of players in the game
- `goldRemaining`: an integer representing the amount of gold left in the map

### Definition of function prototypes

#### `grid_init`- initializes a grid structure given a map file
```c
grid_t* grid_init(FILE* inputMap);
```

#### `grid_placeGold`- places the gold piles randomly in the map
```c
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int goldTotal, int seed);
```

#### `grid_addPlayer`- Adds a player to the grid
```c
bool grid_addPlayer(grid_t* grid, player_t* newPlayer);
```
### Detailed pseudo code

#### `grid_init`

This function loads the grid object from a map.txt file

Pseudocode:

	make a new grid object
	make new array with 26 spots
	set the remaining gold to be `GoldTotal`
	set the numRows and numColumns variables in the new grid to represent the map
	loop through each line of the file
		add that line to the grid's map string
		add the new line character at the end 
	return the grid

#### `grid_placeGold` 

This function loads the gold piles into the map randomly

Pseudocode:

	create a new counter for the gold
	if there is a seed given, call srand(seed), otherwise call srand(getpid())
	find the how much gold per pile if we have the max number of piles 
	find the how much gold per pile if we have the min number of piles 
	while gold on the map is less than 250 - the max gold per pile
		get a random character
		if that character is a .
			put an random amount of gold in between those two numbers the pile
			update the gold counter
			change the character in the map
			increment
	find another random char
		if it is a .	
			add the rest of the remaining gold there so 250 has been placed
			follow the same process for adding as above
	return true

#### `grid_addPlayer`

This function takes a player struct and adds it to the map

Pseudocode:

	copy that player into the grid's player array
	put that player into the random location on the map
	initialize any other player parameters, e.g.: player's position

#### `grid_delete`

This function deletes the grid as well as all players. 

	loop all players in the playerArray
		delete the player
	delete the counter: goldTable
	delete the map: gridString
	delete the grid

#### `addPlayerInArray`

This is the helper function to add a player in grid.
	
	loop all entries in the playerArray,
		if there's an empty spot, 
			add the player in this spot
			return true 
		else, 
			return false

---

## Testing plan

To test the grid, we will use first a series of unit tests, then integration testing with just the server module, and finally system testing with the server and client. 

### unit testing

#### player

We test the player as follows:

- Test `player_init` function to create a player.
- Test `player_updateVisibility` by moving the player through keyboard, both available spots and walls 
- Test `player_updateSpecVisibility` by creating a spectator and show its visibility
- Test `player_move` to by moving the player through keyboard, both available spots and walls 

#### grid

We test the grid as follows: 

The grid functions were tested in the `gridtest.c` file:
- create a grid 
- place some gold
- print out the grid table
- add player into grid 

### integration testing
Then the integration tests will be done likely by observing behavior of the server run with the grid. 

### system testing
Our system testing will involve rigorous testing with the server, client, and multiple players to ensure proper function and error handling. 
We will also likely try to use a test program to send messages to the server via client or even pre-code certain scenarios or edge cases in to demonstrate funcitonality. 

---
