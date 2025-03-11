#ifndef SNAKE_MAIN_H_
#define SNAKE_MAIN_H_

#include "snake.h"

enum Marker { WALL='X', SNAKE='*', FRUIT='@', EMPTY=' ' };

/* Function prototypes */
struct Coordinates new_fruit(char game_board[MAX_ROWS][MAX_COLS + 2]);
void print_game_board(char game_board[MAX_ROWS][MAX_COLS + 2]);
enum Direction decode_input(int ch, enum Direction current_direction);
int update_snake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]);
void print_game_over(struct Snake *snake);
void print_game_start();
void print_score(int score);

#endif /* SNAKE_MAIN_H_ */
