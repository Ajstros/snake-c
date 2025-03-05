#ifndef SNAKE_MAIN_H_
#define SNAKE_MAIN_H_

#include "snake.h"

enum Marker { WALL='X', SNAKE='*', FRUIT='@', EMPTY=' ' };

/* Function prototypes */
// Create a new fruit in a random empty space on the game board. Returns the
// coordinates of that new fruit.
struct Coordinates new_fruit(char game_board[MAX_ROWS][MAX_COLS + 2]);
// Print the game board to the screen at (0,0)
void print_game_board(char game_board[MAX_ROWS][MAX_COLS + 2]);
// Decode the user input after getch() to return the new snake direction.
// Defaults to the supplied currentDirection if no arrow keys or WASD pressed.
enum Direction decode_input(int ch, enum Direction current_direction);
// Update the snake's position on the game board. The snake moves in its
// current direction, then we check for collisions and update the board
// accordingly. Returns 0 for end of game, 1 to continue.
int update_snake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]);

#endif // SNAKE_MAIN_H_
