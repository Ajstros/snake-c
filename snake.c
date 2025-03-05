#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "snake.h"

/* Push new coordinates into the snake's body Queue. Returns the end of the snake's coordinates as they are pushed out of the Queue */
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord) {
  struct Node *exit_node;
  struct Coordinates exit_data;
  struct Node *new_node = malloc(sizeof(struct Node));
  // Must dynamically allocate so data is not lost when this function returns
  new_node->data = new_coord;
  new_node->next = snake->body.head;
  new_node->prev = NULL;
  // Set new node as head
  snake->body.head->prev = new_node;
  snake->body.head = new_node;

  // Remove last node, return data
  exit_node = snake->body.tail;
  exit_data = exit_node->data;
  snake->body.tail = exit_node->prev;  // Tail = Tail - 1
  free(exit_node);
  return exit_data;
}

/* Add new coordinates to the snake's body Queue. The Queue is made larger so no coordinates are lost. Used for updating the snake when it eats a fruit. */
void snake_add(struct Snake *snake, struct Coordinates new_coord) {
  struct Node exit_node;
  // Must dynamically allocate so data is not lost when this function returns
  struct Node *new_node = malloc(sizeof(struct Node));
  new_node->data = new_coord;
  new_node->next = snake->body.head;
  new_node->prev = NULL;
  // Set new node as head. Do not remove last node
  snake->body.head->prev = new_node;
  snake->body.head = new_node;
  snake->length++;
}

/* Initialize Snake structure. Snake starts in the middle of the grid defined by MAX_ROWS, MAX_COLS with a length of 1 moving UP. */
struct Snake initSnake() {
  struct Coordinates init_coord = {MAX_ROWS / 2, MAX_COLS / 2};
  return (struct Snake){
    .body = initQueue(init_coord),
    .length = 1,
    .direction = UP,
  };
}
