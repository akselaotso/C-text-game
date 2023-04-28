# C-text-game

A simple text game in C. The project required use of functions, memory management and basic programming project planning and implementation. This was the final project for out Basics in C Programming -course.

The system has the following commands:
Every action prints "SUCCESS\n" after being completed, or an error message accompanied by "Invalid command <command>".

```
A <name> <healthPoints> <weaponName> <damage>
```
Creates a new character with the specified stats.

```
H <attacker> <target>
```
Attacks a character, the attacker recieves XP equal to the damage dealt. Prints out the following:

Bilbo attacked Smeagol with Dagger by 6 damage.  
Smeagol has 8 hit points remaining.   
Bilbo gained 6 experience points.   
SUCCESS

A character dies if their healthpoints reach 0.

```
L
```
Prints the current gamestate in format "<name> <hit-points> <experience> <weapon-name> <weapon-damage>\n", for example
  
Bilbo 12 6 Dagger 6  
Bilbo 12 6 Dagger 6  
SUCCESS

```
W <filename>
```
Saves the current gamestate in a file in the same format as the printout.

```
O <filename>
```
Loads a gamestate from a file, overwriting the current state.

```
Q
```
Ends the game.
