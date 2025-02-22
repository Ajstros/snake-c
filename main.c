#include <ncurses.h>
#include <stdlib.h>

#define MAX_ROWS 10
#define MAX_COLS 10

struct Coordinates {
  int row;
  int col;
};

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Marker { SNAKE, FRUIT, EMPTY=0 };

struct Snake {
  struct Coordinates head;
  struct Coordinates tail;
  int length;
  enum Direction direction;
};

// Function prototypes
struct Snake initSnake();
struct Coordinates newFruit(enum Marker game_board[MAX_ROWS][MAX_COLS]);
void printGameBoard(enum Marker game_board[MAX_ROWS][MAX_COLS]);
int getKeyPress();

int main(int argc, char *argv[]) {
  int ch;
  struct Snake snake;
  struct Coordinates fruit;
  enum Marker game_board[MAX_ROWS][MAX_COLS];

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  /*1. Press enter to start game*/
  while (getch() != '\n');

  /*2. Generate fruit*/
  snake = initSnake();
  game_board[snake.head.row][snake.head.col] = SNAKE;
  fruit = newFruit(game_board);
  game_board[fruit.row][fruit.col] = FRUIT;

  /*3. Show game board*/
  printGameBoard(game_board);
  getch();

  /*4. Move snake according to arrow keys or WASD*/


  /*5. Check for snake collision with fruit, edges of screen, itself*/

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

struct Coordinates newFruit(enum Marker game_board[MAX_ROWS][MAX_COLS]) {
  struct Coordinates fruit;

  do {
    fruit.row = rand() % MAX_ROWS;
    fruit.col = rand() % MAX_COLS;
  } while (game_board[fruit.row][fruit.col] != EMPTY);

  return fruit;
}

void printGameBoard(enum Marker game_board[MAX_ROWS][MAX_COLS]) {
  // Top
  for (int i=0; i < MAX_ROWS; i++) {
    printw("X");
  }
  printw("\n");
  // Walls
  for (int j=1; j < MAX_COLS - 1; j++) {
    printw("X");
    for (int i=1; i < MAX_ROWS - 1; i++) {
      printw(" ");
    }
    printw("X");
    printw("\n");
  }
  // Bottom
  for (int i=0; i < MAX_ROWS; i++) {
    printw("X");
  }
  printw("\n");
  refresh();
}

int getKeyPress() {

  return 0;
}
