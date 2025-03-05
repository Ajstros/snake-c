#ifndef SNAKE_MAIN_H_
#define SNAKE_MAIN_H_

#include "snake.h"

enum Marker { WALL='X', SNAKE='*', FRUIT='@', EMPTY=' ' };

// Function prototypes
struct Coordinates newFruit(char game_board[MAX_ROWS][MAX_COLS + 2]);
void printGameBoard(char game_board[MAX_ROWS][MAX_COLS + 2]);
enum Direction decodeInput(int ch, enum Direction currentDirection);
int updateSnake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]);

#endif // SNAKE_MAIN_H_
