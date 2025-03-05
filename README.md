Snake game.

Want snake to go around the screen in a terminal and eat fruits.

# Build Instructions
Use `make` to build and run the program. Use `make debug` to build and enter
GDB.

# Snake
Create snake with icon *
* Snake is connected
* If snake hits itself, game ends
* If snake would go offscreen, game ends

# Fruit
Create fruit with icon @
* Track score by number of fruits eaten
* Snake gets 1 * bigger for every fruit eaten
* Fruit placement is random

# Planning
Need to:

1. Press enter to start game
2. Generate fruit
3. Show game board
4. Move snake according to arrow keys or WASD
5. Check for snake collision with fruit, edges of screen, itself
    * Fruit -> Make snake one longer by not deleting the last element (tail) of
    the snake when we update the screen
    * Edge, itself -> End game, show score
6. Restart game by pressing enter after game ends

# Implementation
* Used a Queue structure to store the snake's body
    * This makes it easy to keep track of the head (new snake body that must be
    added) and tail (old snake body that must be removed) coordinates as the
    snake moves
    * _Note_: there are a lot of pointers involved here. I ran into a lot of
    issues when I tried to return pointers to new variables that I created from
    a function. The problem is, that pointer gets returned but the data it
    pointed to was only in the scope of that function. So, once the function
    returns, the pointer points to garbage. When I tried to use that pointer to
    get at the data I created earlier, the garbage I got instead led to a
    segfault.

# File Structure
- src/
    - main.h
    - snake.h
    - queue.h
    - main.c
    - snake.c
    - queue.c
    - Makefile
