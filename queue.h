#ifndef SNAKE_QUEUE_H_
#define SNAKE_QUEUE_H_

struct Coordinates {
  int row;
  int col;
};

struct Node {
  struct Node *next;
  struct Node *prev;
  struct Coordinates data;
};

struct Queue {
  struct Node *head;
  struct Node *tail;
  int length;
};

/* Function prototypes */
/**
 * \brief           Initialize the Queue with one node containing initial coordinates
 * \param[in]       init_coord: initial coordinates for the first node
 * \return          Initialized Queue struct
 */
struct Queue init_queue(struct Coordinates init_coord);

#endif /* SNAKE_QUEUE_H_ */
