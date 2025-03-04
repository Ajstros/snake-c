#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROWS 18
#define MAX_COLS 36

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

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Marker { WALL='X', SNAKE='*', FRUIT='@', EMPTY=' ' };

struct Snake {
  struct Queue body;
  int length;
  enum Direction direction;
};

// Function prototypes
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord);
void snake_add(struct Snake *snake, struct Coordinates new_coord);
struct Snake initSnake();
struct Coordinates newFruit(char game_board[MAX_ROWS][MAX_COLS + 2]);
void printGameBoard(char game_board[MAX_ROWS][MAX_COLS + 2]);
enum Direction decodeInput(int ch, enum Direction currentDirection);
int updateSnake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]);

int main(int argc, char *argv[]) {
  int running = 1;
  int updateTime = 200;
  int updateCount = 0;
  int waitInterval = 20;
  int ch;
  struct Snake snake;
  struct Coordinates fruit;
  char game_board[MAX_ROWS][MAX_COLS + 2];
  for (int i=0; i < MAX_ROWS; i++) {
    for (int j=0; j < MAX_COLS; j++) {
      if ((i == 0) || (j == 0) || (i == MAX_ROWS - 1) || (j == MAX_COLS - 1)) {
        game_board[i][j] = WALL;
      } else {
        game_board[i][j] = EMPTY;
      }
    }
    game_board[i][MAX_COLS] = '\n';
    game_board[i][MAX_COLS + 1] = '\0';
  }

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);

  /*1. Press enter to start game*/
  printw("Welcome\n");
  printw("to\n");
  printw("Snake\n");
  while (getch() != '\n');

  /*2. Generate fruit*/
  snake = initSnake();
  game_board[snake.body.head->data.row][snake.body.head->data.col] = SNAKE;
  fruit = newFruit(game_board);

  /*3. Show game board*/
  printGameBoard(game_board);
  getch();

  while (running) {
    /*4. Move snake according to arrow keys or WASD*/
    ch = getch();
    snake.direction = decodeInput(ch, snake.direction);
    napms(waitInterval);
    updateCount += waitInterval;
    if (updateCount >= updateTime) {
      updateCount = 0;
      /*5. Check for snake collision with fruit, edges of screen, itself*/
      running = updateSnake(&snake, game_board);
      printGameBoard(game_board);
    }
  }

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}

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

struct Queue initQueue(struct Coordinates init_coord) {
  // Must dynamically allocate so data is not lost when this function returns
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

struct Snake initSnake() {
  struct Coordinates init_coord = {MAX_ROWS / 2, MAX_COLS / 2};
  return (struct Snake){
    .body = initQueue(init_coord),
    .length = 1,
    .direction = UP,
  };
}

struct Coordinates newFruit(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates fruit;
  srand(time(NULL));

  do {
    fruit.row = rand() % MAX_ROWS;
    fruit.col = rand() % MAX_COLS;
  } while (game_board[fruit.row][fruit.col] != EMPTY);

  game_board[fruit.row][fruit.col] = FRUIT;

  return fruit;
}

void printGameBoard(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  move(0, 0);
  for (int i=0; i < MAX_ROWS; i++) {
    printw("%s", game_board[i]);
  }

  refresh();

  return;
}

enum Direction decodeInput(int ch, enum Direction currentDirection) {
  switch (ch) {
    // Arrow keys
    case 259:
      return UP;
      break;
    case 258:
      return DOWN;
      break;
    case 261:
      return RIGHT;
      break;
    case 260:
      return LEFT;
      break;
    // WASD
    case 'w':
      return UP;
      break;
    case 's':
      return DOWN;
      break;
    case 'd':
      return RIGHT;
      break;
    case 'a':
      return LEFT;
      break;
    default:
      break;
  }
  return currentDirection;
}

int updateSnake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates prev_tail_coord;
  struct Coordinates next_head_coord = snake->body.head->data;
  // Remove tail from game board (must do before checking for collisions to prevent colliding with where our tail used to be)
  // Save the coordinates so we can add it back if we collide with a fruit
  prev_tail_coord = snake->body.tail->data;
  game_board[prev_tail_coord.row][prev_tail_coord.col] = EMPTY;
  // Calculate new head coordinates
  switch (snake->direction) {
    case UP:
      next_head_coord.row  = snake->body.head->data.row - 1;  // Row 0 is at top of screen
      break;
    case DOWN:
      next_head_coord.row  = snake->body.head->data.row + 1;  // Row 0 is at top of screen
      break;
    case RIGHT:
      next_head_coord.col  = snake->body.head->data.col + 1;
      break;
    case LEFT:
      next_head_coord.col  = snake->body.head->data.col - 1;
      break;
  }

  // Check for collisions (apple, wall, self)
  // Return 0 to stop game, 1 to continue
  switch (game_board[next_head_coord.row][next_head_coord.col]) {
    case SNAKE:
      return 0;
      break;
    case WALL:
      return 0;
      break;
    case FRUIT:
      // Update snake
      snake_add(snake, next_head_coord);
      // Add new snake head
      game_board[next_head_coord.row][next_head_coord.col] = SNAKE;
      // Add old tail back
      game_board[prev_tail_coord.row][prev_tail_coord.col] = SNAKE;
      // Draw new fruit
      newFruit(game_board);
      break;
    default:
      // Catches empty as well
      // Update as normal
      snake_push(snake, next_head_coord);
      // Add new snake head
      game_board[next_head_coord.row][next_head_coord.col] = SNAKE;
      break;
  }

  return 1;
}
