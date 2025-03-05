#include "snake.h"
#include <stdlib.h>
#include <time.h>
#include "queue.h"

struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord) {
  struct Node *exit_node;
  struct Coordinates exit_data;
  struct Node *new_node = malloc(sizeof(struct Node));
  /* Must dynamically allocate so data is not lost when this function returns */
  new_node->data = new_coord;
  new_node->next = snake->body.head;
  new_node->prev = NULL;
  /* Set new node as head */
  snake->body.head->prev = new_node;
  snake->body.head = new_node;

  /* Remove last node, return data */
  exit_node = snake->body.tail;
  exit_data = exit_node->data;
  snake->body.tail = exit_node->prev;  /* Tail = Tail - 1 */
  free(exit_node);
  return exit_data;
}

void snake_add(struct Snake *snake, struct Coordinates new_coord) {
  struct Node exit_node;
  /* Must dynamically allocate so data is not lost when this function returns */
  struct Node *new_node = malloc(sizeof(struct Node));
  new_node->data = new_coord;
  new_node->next = snake->body.head;
  new_node->prev = NULL;
  /* Set new node as head. Do not remove last node */
  snake->body.head->prev = new_node;
  snake->body.head = new_node;
  snake->length++;
}

struct Snake init_snake() {
  struct Coordinates init_coord = {MAX_ROWS / 2, MAX_COLS / 2};
  return (struct Snake){
    .body = init_queue(init_coord),
    .length = 1,
    .direction = UP,
  };
}
