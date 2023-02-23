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

## Grid

The grid module ultimately stores and maintains the game map itself, containing methods allowing for easy access and updates to information in the map. 
> List and briefly describe any modules that comprise your client, other than the main module.
 
> Repeat this section for each module that is included in either the client or server.

### Functional decomposition


> List each of the main functions implemented by this module, with a phrase or sentence description of each.

### Pseudo code for logic/algorithmic flow

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
