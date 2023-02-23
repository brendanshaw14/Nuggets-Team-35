# CS50 Nuggets
## Design Spec
### Team 35, Winter, 2023

Team Members: Ming Cheng, Romeo Myrthil, Brendan Shaw

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes `player` and `grid` modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

### User interface

Because we have only three group members, we will be using the `client` executable given to us in the `shared` library instead of writing out own, which takes two three arguments: the 

 > Example: ./client hostname port [playername] 

See the requirements spec for both the command-line and interactive UI.

### Inputs and outputs- Keystrokes and Display

* `Q` : quit- turns off display and exits the client. 
* `h` : move left- moves the player's location and icon to the left, if possible.  
* `l` : move right- moves the player's location and icon to the right, if possible.  
* `j` : move down- moves the player's location and icon down, if possible.  
* `k` : move up- moves the player's location and icon up, if possible.  
* `y` : move diagonally up and left- moves the player's location and icon diagonally and to the left, if possible.  
* `u` : move diagonally up and right- moves the player's location and icon diagonally and to the right, if possible.  
* `b` : move diagonally down and left- moves the player's location and icon diagonally and to the left, if possible.  
* `n` : move diagonally down and left- moves the player's location and icon diagonally and to the left, if possible.  

NEED TO INSERT LOG INFO HERE
> Briefly describe the inputs (keystrokes) and outputs (display).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.

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

All messages will be written to a log file. 

> Briefly describe the inputs (map file) and outputs (to terminal).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.

### Functional decomposition into modules
* parseArgs- handles and verifies arguments
* initializeGame- configures the `game` struct and loads the grid
* gameOver- informs all clients that the game has ended
> List and briefly describe any modules that comprise your server, other than the main module.

### Pseudo code for logic/algorithmic flow

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up


> Then briefly describe each of the major functions, perhaps with level-4 #### headers.

### Major data structures

> Describe each major data structure in this program: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).

---

## XYZ module (remove this line after adding all modules)

## Player

The player module aims to collect and manage the information of all players. 

It mainly contains the following:
```c
struct Player {
	int player_position; 
	char* player_name; 
	int player_amountOfGold; 
}; 
```

1. `player_position` is the index of the grid string to represent the position of current player. For example: `grid_string[index]`.

2. `player_name` is the string indicating the player's name.

3. `player_amountOfGold` is an integer indicating the amount of gold found by current player. Every time a player finds a gold, we should update its value.

The functions it contains are listed below.

### Functional decomposition

#### `player_getName`

The definition of this function is `char* player_getName();`. It returns the name of the current player. 

#### `player_getPosition`

The definition of this function is `int player_getPosition();`. It returns an integer, as an index of the string, to represent the position of current player. 

#### `player_getGold`

The definition of this function is `int player_getGold();`. It returns the amount of golds found by the current player. 

#### `player_move`

The definition of this function is `bool player_move(char k);`. It returns `true` when the input character `k` is valid, and vice versa.


### Pseudo code for logic/algorithmic flow

#### `player_getName`

```c
	return Player->player_name; 
```

#### `player_getPosition`

```c
	return Player->player_position; 
```

#### `player_getGold`

```c
	return Player->player_amountOfGold; 
```

#### `player_move` 

```c
	if input char is not valid:
		return false; 
	else, parse the char:
		if new position is not accessable (e.g. wall, spaces):
			return true; 
		else: 
			update the player's position
			return true; 
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
	bag_t* players; 
	int goldTotalLeft; 
	hashtable_t* goldLocation;
}; 
```

> List and briefly describe any modules that comprise your client, other than the main module.
 
> Repeat this section for each module that is included in either the client or server.

### Functional decomposition

* `grid_init` - reads a map into a string and returns a grid_t structure with the string, and the width and height of the map.
* `grid_placeGold` - takes the grid and the gold constants and returns a hashtable of gold (where the index to a point in the string is the key, and the amount of gold is the item).
* `grid_getGoldLeft` - return the amount of gold left to claim on the map.

> List each of the main functions implemented by this module, with a phrase or sentence description of each.

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

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

#### Hashtable 

A data structure `hashtable` will be used to store the locations and amounts of gold piles across the map. The key would in this case be an index in the map string, which would be a spot on the map, and the item would an amount of gold stored as an int.

#### Bag

A data structure `bag` will be used to store the players of the game. The key is the name of the player that the client joins with, and the item is a `player_t` struct storing position, the name, and the amount of gold

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
