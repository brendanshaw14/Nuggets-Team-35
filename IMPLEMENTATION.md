# CS50 Nuggets
## Implementation Spec
### Team name, term, year

> This **template** includes some gray text meant to explain how to use the template; delete all of them in your document!

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

> Teams of 3 students should delete this section.

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
	initialize message module
	print assigned port number
	decide whether spectator or player

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
	for players in the grid->players hashtable
	  message_send to players address, with timeout message
	// extend on closing the server. do i make each player quit or does quitting the server  
	close the server // game over function?
	return true // to close the message_loop? dont know if true or false
```

##### `handleInput`

##### `handleMessage`

Handles the different kinds of messages sent by clients and runs as follows:

```c
	if message is PLAY
	  initialize a new player struct, initializing adress position and name
	  if numPlayers == maxPlayers
	  	message_send to client saying server is full
		return false  // to continue loop
	  add player to players hashtable
	  increment numPlayers and assign corresponding letter
	  message_send OK letter, where letter is the assigned letter
	  message_send GRID nrows ncols to inform player of map size 
	  // do i send initial display here too?
	if message is SPECTATE
	  if there is another spectator // how to implement this? doesnt really work with bag
	  	message_send QUIT to that player/spectator
		remove from hashtable
		initialize a new player struct, using spectator_init
		message_send GRID with full grid to spectator
	if message is KEY
	  will write this soon
	if all gold was collected
	  will write this soon
```

###### player_init

A helper function for initializing the players

```c
	not written yet
```

###### spectator_init

A helper function for initializing the spectator

```c
	not written yet
```

---

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
