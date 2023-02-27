# CS50 Nuggets
## Implementation Spec
### Team 35, Winter, 2023

Team Members: Ming Cheng, Romeo Myrthil, Brendan Shaw

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

> Update your plan for distributing the project work among your 3(4) team members.
> Who writes the client program, the server program, each module?
> Who is responsible for various aspects of testing, for documentation, etc?

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
player_t* player_init(char* map); 
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

	allocate space for the structure
	set initial value of variables in the structure
	set the initial value of player_seen string


#### `player_updateVisibility`

	get the current position of player
	loop for all positions in the map:
		for each position, compute the distance between it and player
		if the distance is less than radius:
			set it to be visible (map[position])
	set the new player position to '*' to mark it's a player

#### `player_move`

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
	if current position is '.' or '#':
		return true
	else:
		return false

---

## Server

### Data structures

> For each new data structure, describe it briefly and provide a code block listing the `struct` definition(s).
> No need to provide `struct` for existing CS50 data structures like `hashtable`.

### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
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
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

#### `message_loop`

The function `message_loop` given to us by the message.c file in the support folder, provides us with the ability to wait for messages sent to the server by the client. The function takes 3 helper functions as arguments and the implementation of these will be described below.

##### `handleTimeout`

After a desired amount of time `const float timeout` without a message, `message_loop` calls `handleTimeout` which runs as follows:

```c
	print timeout message to server
	iterate thorugh players in hashtable using endGame_helper 
	close the server 
	return true 
```

##### `handleInput`

##### `handleMessage`

Handles the different kinds of messages sent by clients and runs as follows:

```c
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
```

###### spectator_init

A helper function for initializing the spectator

```c
	alloc memory for spectator player struct
	spectator->player_address = address
	get spectator name from the message string in handleMessage
	spectator->player_seen = Grid->grid  // the total map
	return spectator player struct
```

###### updateVisibilities_helper

A helper function for hashtable iterate, for updating every players visibility and sending it to their clients

```c
	player_t player = item
	update player visibility
	player->player_seen = that new visibility
	message_send DISPLAY\nplayer->player_seen
```
---

###### updateGoldStatus_helper

A helper function for hashtable iterate, for sending out a message about the goldStatus to every player

```c
	player_t player = item
	message_send GOLD n p r 
	where n is the nuggets just picked up, p is players total nuggets, and r is remaining on the map
```

###### endGame_helper

A helper function for hashtable iterate, for helping each client quit the game 

```c
	player_t player = item
	hashtable_t players = arg
	message_send QUIT for that player
	delete player from players
	free player
```


## XYZ module

> For each module, repeat the same framework above.

### Data structures

### Definition of function prototypes

### Detailed pseudo code

---

## Testing plan

### unit testing

> How will you test each unit (module) before integrating them with a main program (client or server)?

### integration testing

> How will you test the complete main programs: the server, and for teams of 4, the client?

### system testing

> For teams of 4: How will you test your client and server together?

---

## Limitations

> Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation.
