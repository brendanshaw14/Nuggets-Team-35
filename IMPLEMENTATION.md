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

> For function, provide a brief description and then a code block with its function prototype.
> For example:

#### `parseArgs`- A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
```
#### `initializeGame`- A funciton to inialize the grid module and the game structure's members. 
```c
bool initializeGame(const int argc, char* argv[]);
```

### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

#### `parseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		save the seed	
	else
		save -1 	

#### `initializeGame`

The function `initializeGame` takes the arguments validated by parseArgs to fully initialize the game structure within the server, as well as the grid module. 

	initialize the game struct and its members
	call grid init 
	initialize the network

#### `message_loop`

The function `message_loop` given to us by the message.c file in the support folder, provides us with the ability to wait for messages sent to the server by the client. The function takes 3 helper functions as arguments and the implementation of these will be described below.

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
    int player_address; 
    int player_position; 
    char* player_name; 
    int player_amountOfGold; 
    char* player_seen; 
} player_t;
```

Currently, a `player` structure contains the following: 
- `player_address`: an integer representing player's address
- `player_position`: an integer representing player's position
- `player_name`: a string representing player's name
- `player_amountOfGold`: an integer representing the amount of gold that current player has found
- `player_seen`: a string representing the player's current visibility


### Definition of function prototypes

```c
player_t* player_init(char* map, int address, int init_position, char* name, bool isSpectator); 
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
void player_updateVisibility(player_t* player, char* map, int width, double radius); 
```
`player_updateVisibility` function is used to update the visible range when player moves.


```c
bool player_move(player_t* player, char k, int width, int height, char* map, double radius);
```

`player_move` function is used to move the player (update the player's position), and calls `player_updateVisibility` function to update the visible range.


### Detailed pseudo code

#### `player_init`:

Initializes a player structure, setting default values and the players name, address, and info, calls the `grid_addPlayerMethod`.

	allocate space for the structure
	set initial value of variables in the structure
	set the initial value of player_seen string
	set the default player values
	add the player to the grid (grid_addPlayer)

#### `player_updateVisibility`

Retrieves the player's current visibility screen. 

	get the current position of player
	loop for all positions in the map:
		for each position, compute the distance between it and player
		if the distance is less than radius:
			set it to be visible (map[position])
	set the new player position to '*' to mark it's a player

#### `player_move`

Moves the player on the grid given the input key.

	check the input key, if it's valid:
		check whether current position reaches bound
		if so: 
			directly return true
		else：
			move to new position:
				check if new position is able to move to
				if so: 
					update the player's position, return true
				else:
					return false
			if `moveToNewPosition` returns `true`: 
			update the player's visibility
			update the amount of gold if new position is gold
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

	check whether current position is valid
	if so: 
		reset the `player_seen`
		update the `player_position`
		return true
	else:
		return false 

#### `checkValidPosition`
Makes sure that the player is in a valid position. 
	if current position is '.' or '#':
		return true
	else:
		return false

#### updateVisibilities_helper

A helper function for hashtable iterate, for updating every players visibility and sending it to their clients

	player_t player = item
	update player visibility
	player->player_seen = that new visibility
	message_send DISPLAY\nplayer->player_seen

#### updateGoldStatus_helper

A helper function for hashtable iterate, for sending out a message about the goldStatus to every player

	player_t player = item
	message_send GOLD n p r 
	where n is the nuggets just picked up, p is players total nuggets, and r is remaining on the map

#### endGame_helper

A helper function for hashtable iterate, for helping each client quit the game 

	player_t player = item
	hashtable_t players = arg
	message_send QUIT for that player
	delete player from players
	free player

---
## Grid

The grid module holds most of the important data for the game, including the map, a `hashtable of `player` structures, a `counters` representing the gold piles on the map. 

```c
typedef struct grid {
	char* gridString;
    hashtable_t* playerTable;
    counters_t* goldTable;
    int numRows;
    int numColumns;
    int numPlayers;
    int goldRemaining;
} grid_t;
```

Currently, a `grid` structure contains the following: 
- `gridString`: a string representing the entire map
- `playerTable`: a hashtable of player objects representing the players in the game
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
bool grid_placeGold(grid_t* grid, int minPiles, int maxPiles, int seed);
```

#### `grid_addPlayer`- Adds a player to the grid
```c
bool grid_addPlayer(grid_t* gid, player_t* player);
```
### Detailed pseudo code

#### `grid_init` - loads the grid object from a map.txt file
Pseudocode:
	make a new grid object
	make new hashtable with 26 spots
	set the numRows and numColumns variables in the new grid to represent the map
	loop through each line of the file
		add that line to the grid's map string
		add the new line character at the end 
	return the grid

#### `grid_placeGold` - loads the gold piles into the map randomly
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

#### `grid_addPlayer` - takes a player struct and adds it to the map
Pseudocode:
	copy that player into the grid's player hashtable
	put that player into the random location on the map
	initialize any other player parameters
	generate the player's letter and display it on the map
	
---

## Testing plan

To test the grid, we will use first a series of unit tests, then integration testing with just the server module, and finally system testing with the server and client. 

### unit testing
The unit tests will be executed using the gridtest.c program in the grid directory, where we will handle edge cases and error handling. 

### integration testing
Then the integration tests will be done likely by observing behavior of the server run with the grid. 

### system testing
Our system testing will involve rigorous testing with the server, client, and multiple players to ensure proper function and error handling. 
We will also likely try to use a test program to send messages to the server via client or even pre-code certain scenarios or edge cases in to demonstrate funcitonality. 

---

## Limitations

> Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.
