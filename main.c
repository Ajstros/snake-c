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
      running = updateSnake(&snake, game_board);
      printGameBoard(game_board);
    }

    /*5. Check for snake collision with fruit, edges of screen, itself*/

  }

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}

/* Push new coordinates into the snake's body Queue. Returns the end of the snake's coordinates as they are pushed out of the Queue */
struct Coordinates snake_push(struct Snake *snake, struct Coordinates new_coord) {
  struct Node exit_node;
  struct Node new_node = {
    .data = new_coord,
    .next = snake->body.head,
    .prev = &new_node
  };
  // Set new node as head
  snake->body.head->prev = &new_node;
  snake->body.head = &new_node;

  // Remove last node, return data
  exit_node = *(snake->body.tail);
  snake->body.tail = exit_node.prev;
  exit_node.prev = &exit_node;
  return exit_node.data;
}

/* Add new coordinates to the snake's body Queue. The Queue is made larger so no coordinates are lost. Used for updating the snake when it eats a fruit. */
void snake_add(struct Snake *snake, struct Coordinates new_coord) {
  struct Node exit_node;
  struct Node new_node = {
    .data = new_coord,
    .next = snake->body.head,
    .prev = &new_node
  };
  // Set new node as head. Do not remove last node
  snake->body.head->prev = &new_node;
  snake->body.head = &new_node;
  snake->length++;
}

struct Queue initQueue(struct Coordinates init_coord) {
  struct Node n = {
    .data = init_coord,
    .next = &n,
    .prev = &n
  };
  return (struct Queue){
    .head = &n,
    .tail = &n,
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
    printw("%s", (char *)game_board[i]);
  }

  refresh();
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
  struct Coordinates tail_coord;
  struct Coordinates head_coord;
  // Remove tail (must do before checking for collisions to prevent colliding with where our tail used to be)
  // Save the coordinates so we can add it back if we collide with a fruit
  tail_coord = snake->body.tail->data;
  game_board[tail_coord.row][tail_coord.col] = EMPTY;
  snake->body.tail = snake->body.tail->prev;  // Reassign tail to 2nd to last node
  snake->body.tail->next = snake->body.tail;  // Remove previous tail from next reference
  // Update body.head.data
  switch (snake->direction) {
    case UP:
      snake->body.head->data.row--;  // Row 0 at top of screen
      break;
    case DOWN:
      snake->body.head->data.row++;  // Row 0 at top of screen
      break;
    case RIGHT:
      snake->body.head->data.col++;
      break;
    case LEFT:
      snake->body.head->data.col--;
      break;
  }

  // Check for collisions (apple, wall, self)
  // Return 0 to stop game, 1 to continue
  switch (game_board[tail_coord.col][snake->body.head->data.col]) {
    case SNAKE:
      return 0;
      break;
    case WALL:
      return 0;
      break;
    case FRUIT:
      // Update snake
      snake->length++;
      snake->body.tail = &((struct Node) {
        .prev = snake->body.tail,
        .data = tail_coord
      });
      // Draw new fruit
      newFruit(game_board);
      break;
    default:
      // Catches empty as well
      break;
  }

  game_board[snake->body.head->data.row][snake->body.head->data.col] = SNAKE;
  return 1;
}
