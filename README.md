Snake game.

Want snake to go around the screen in a terminal and eat fruits.

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
2. Show snake
3. Generate fruit
4. Move snake according to arrow keys or WASD
5. Check for snake collision with fruit, edges of screen, itself
    * Fruit -> Make snake one longer by not deleting the last element (tail) of the snake when we update the screen
    * Edge, itself -> End game, show score
6. Restart game by pressing enter after game ends
