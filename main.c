#include <curses.h>
#include <stdlib.h>

#define MAX_ROWS 18
#define MAX_COLS 36

struct Coordinates {
  int row;
  int col;
};

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Marker { WALL='X', SNAKE='*', FRUIT='@', EMPTY=' ' };

struct Snake {
  struct Coordinates head;
  struct Coordinates tail;
  int length;
  enum Direction direction;
};

// Function prototypes
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
  game_board[snake.head.row][snake.head.col] = SNAKE;
  fruit = newFruit(game_board);
  game_board[fruit.row][fruit.col] = FRUIT;

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

struct Snake initSnake() {
  return (struct Snake){
    .head = {MAX_ROWS / 2, MAX_COLS / 2},
    .tail = {MAX_ROWS / 2, MAX_COLS / 2},
    .length = 1,
    .direction = UP,
  };
}

struct Coordinates newFruit(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates fruit;

  do {
    fruit.row = rand() % MAX_ROWS;
    fruit.col = rand() % MAX_COLS;
  } while (game_board[fruit.row][fruit.col] != EMPTY);

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
  // Remove tail
  game_board[snake->tail.row][snake->tail.col] = EMPTY;
  snake->tail = snake->head;
  // Update head
  switch (snake->direction) {
    case UP:
      snake->head.row--;  // Row 0 at top of screen
      break;
    case DOWN:
      snake->head.row++;  // Row 0 at top of screen
      break;
    case RIGHT:
      snake->head.col++;
      break;
    case LEFT:
      snake->head.col--;
      break;
  }

  // Check for collisions (apple, wall, self)
  // Return 0 to stop game, 1 to continue
  switch (game_board[snake->head.row][snake->head.col]) {
    case SNAKE:
      return 0;
      break;
    case WALL:
      return 0;
      break;
    case FRUIT:
      // Update snake
      // Draw new fruit
      break;
    default:
      // Catches empty as well
      break;
  }

  game_board[snake->head.row][snake->head.col] = SNAKE;
}
