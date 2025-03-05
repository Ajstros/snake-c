#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "queue.h"

#define MAX_ROWS 18
#define MAX_COLS 36

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Snake {
  struct Queue body;
  int length;
  enum Direction direction;
};

// Function Prototypes
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord);
void snake_add(struct Snake *snake, struct Coordinates new_coord);
struct Snake initSnake();

#endif // SNAKE_SNAKE_H_
