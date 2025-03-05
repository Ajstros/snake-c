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

// Function prototypes
struct Queue initQueue(struct Coordinates init_coord);

#endif // SNAKE_QUEUE_H_
