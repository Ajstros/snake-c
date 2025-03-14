#include "queue.h"
#include <stdlib.h>
#include <time.h>

struct Queue init_queue(struct Coordinates init_coord) {
  /* Must dynamically allocate so data is not lost when this function returns */
  struct Node *n = malloc(sizeof(struct Node));
  n->data = init_coord;
  n->next = NULL;
  n->prev = NULL;
  return (struct Queue){
    .head = n,
    .tail = n,
    .length = 1
  };
}
