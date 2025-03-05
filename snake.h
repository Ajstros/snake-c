#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "queue.h"

#define MAX_ROWS 18
#define MAX_COLS 36

// Direction of Snake movement
enum Direction { UP, DOWN, LEFT, RIGHT };

// Snake player
struct Snake {
  struct Queue body;
  int length;
  enum Direction direction;
};

/* Function Prototypes */
// Push new coordinates into the snake's body Queue. Returns the end of the
// snake's coordinates as they are pushed out of the Queue
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord);
// Add new coordinates to the snake's body Queue. The Queue is made larger so no
// coordinates are lost. Used for updating the snake when it eats a fruit.
void snake_add(struct Snake *snake, struct Coordinates new_coord);
// Initialize Snake structure. Snake starts in the middle of the grid defined by
// MAX_ROWS, MAX_COLS with a length of 1 moving UP.
struct Snake initSnake();

#endif // SNAKE_SNAKE_H_
