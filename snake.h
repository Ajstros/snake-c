#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "queue.h"

#define UPDATE_TIME 200
#define WAIT_INTERVAL 20
#define MAX_ROWS 18
#define MAX_COLS 36

/* Direction of Snake movement */
enum Direction { UP, DOWN, LEFT, RIGHT };

/* Snake player */
struct Snake {
  struct Queue body;
  int length;
  enum Direction direction;
};

/* Function Prototypes */
/**
 * \brief           Push new coordinates into the snake's body Queue
 * \param[in,out]   snake: Snake struct being pushed to
 * \param[in]       new_coord: new coordinates being pushed in
 * \return          End of the snake coordinates pushed out of the Queue
 */
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord);

/**
 * \brief           Add new coordinates to the snake's body Queue
 * \note            The Queue is made larger so no coordinates are lost. Used
 *                      for updating the snake when it eats a fruit.
 * \param[in,out]   snake: Snake struct being pushed to
 * \param[in]       new_coord: new coordinates being pushed in
 */
void snake_add(struct Snake *snake, struct Coordinates new_coord);

/**
 * \brief           Initialize Snake structure in the middle of the grid with length 1 moving UP
 * \returns         Initialized Snake struct
 */
struct Snake init_snake();

#endif /* SNAKE_SNAKE_H_ */
