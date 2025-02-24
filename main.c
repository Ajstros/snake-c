#include <ncurses.h>
#include <stdlib.h>

#define MAX_ROWS 20
#define MAX_COLS 10

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
int getKeyPress();

int main(int argc, char *argv[]) {
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
  raw();
  keypad(stdscr, TRUE);
  noecho();

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

struct Coordinates newFruit(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates fruit;

  do {
    fruit.row = rand() % MAX_ROWS;
    fruit.col = rand() % MAX_COLS;
  } while (game_board[fruit.row][fruit.col] != EMPTY);

  return fruit;
}

void printGameBoard(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  for (int i=0; i < MAX_ROWS; i++) {
    printw("%s", (char *)game_board[i]);
  }

  refresh();
}

int getKeyPress() {

  return 0;
}
