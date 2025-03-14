#include "main.h"
#include "snake.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

static int screen_rows;
static int screen_cols;

int main(int argc, char *argv[]) {
  int update_count = 0;
  int ch;
  struct Snake snake;
  struct Coordinates fruit;
  char game_board[MAX_ROWS][MAX_COLS + 2];

  /* Set up initial game board */
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

  /* Seed random */
  srand(time(NULL));

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);
  getmaxyx(stdscr, screen_rows, screen_cols);

  /*1. Press enter to start game*/
  print_game_start();
  refresh();
  while (getch() != '\n');

  /*2. Generate fruit*/
  snake = init_snake();
  game_board[snake.body.head->data.row][snake.body.head->data.col] = SNAKE;
  fruit = new_fruit(game_board);

  /*3. Show game board*/
  print_game_board(game_board);
  print_score(snake.length);
  refresh();
  getch();

  while (1) {
    /*4. Move snake according to arrow keys or WASD*/
    ch = getch();
    snake.direction = decode_input(ch, snake.direction);
    napms(WAIT_INTERVAL);
    update_count += WAIT_INTERVAL;
    if (update_count >= UPDATE_TIME) {
      update_count = 0;
      /*5. Check for snake collision with fruit, edges of screen, itself*/
      if (update_snake(&snake, game_board)) {
        // Continue Game
        print_game_board(game_board);
        refresh();
      } else {
        // End Game
        print_game_over(&snake);
        refresh();
        break;
      }
    }
  }

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}

/**
 * \brief             Create a new fruit in a random empty game board space
 * \param[in,out]     game_board: the game board array to read and update
 * \return            Coordinates of the new fruit
 */
struct Coordinates new_fruit(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates fruit;

  do {
    fruit.row = rand() % MAX_ROWS;
    fruit.col = rand() % MAX_COLS;
  } while (game_board[fruit.row][fruit.col] != EMPTY);

  game_board[fruit.row][fruit.col] = FRUIT;

  return fruit;
}

/**
 * \brief             Print the game board to the screen at (0,0)
 * \note              Does not call refresh() so multiple prints can happen
 *                    before updating the screen.
 * \param[in,out]     game_board: the game board array to read and update
 */
void print_game_board(char game_board[MAX_ROWS][MAX_COLS + 2]) {
  int start_row = (screen_rows / 2) - (MAX_ROWS / 2);
  int start_col = (screen_cols / 2) - (MAX_COLS / 2);
  for (int i=0; i < MAX_ROWS; i++) {
    mvprintw(start_row + i, start_col, "%s", game_board[i]);
  }

  return;
}

/**
 * \brief             Decode user input after getch() for new snake direction
 * \param[in]         ch: user input from getch()
 * \param[in]         current_direction: current direction of snake, used as default
 * \return            New direction of the snake
 */
enum Direction decode_input(int ch, enum Direction current_direction) {
  switch (ch) {
    /* Arrow keys */
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
    /* WASD */
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
  return current_direction;
}

/**
 * \brief             Update the snake's position on the game board
 * \note              The snake moves in its current direction, then we check
                          for collisions and update the board
 * \param[in,out]     snake: Snake struct being updated
 * \param[in,out]     game_board: the game board array to read and update
 * \return            0 to end game, 1 to continue
 */
int update_snake(struct Snake *snake, char game_board[MAX_ROWS][MAX_COLS + 2]) {
  struct Coordinates prev_tail_coord;
  struct Coordinates next_head_coord = snake->body.head->data;
  /* Remove tail from game board (must do before checking for collisions to prevent colliding with where our tail used to be) */
  /* Save the coordinates so we can add it back if we collide with a fruit */
  prev_tail_coord = snake->body.tail->data;
  game_board[prev_tail_coord.row][prev_tail_coord.col] = EMPTY;
  /* Calculate new head coordinates */
  switch (snake->direction) {
    case UP:
      next_head_coord.row  = snake->body.head->data.row - 1;  /* Row 0 is at top of screen */
      break;
    case DOWN:
      next_head_coord.row  = snake->body.head->data.row + 1;  /* Row 0 is at top of screen */
      break;
    case RIGHT:
      next_head_coord.col  = snake->body.head->data.col + 1;
      break;
    case LEFT:
      next_head_coord.col  = snake->body.head->data.col - 1;
      break;
  }

  /* Check for collisions (apple, wall, self) */
  /* Return 0 to stop game, 1 to continue */
  switch (game_board[next_head_coord.row][next_head_coord.col]) {
    case SNAKE:
      return 0;
      break;
    case WALL:
      return 0;
      break;
    case FRUIT:
      /* Update snake */
      snake_add(snake, next_head_coord);
      /* Add new snake head */
      game_board[next_head_coord.row][next_head_coord.col] = SNAKE;
      /* Add old tail back */
      game_board[prev_tail_coord.row][prev_tail_coord.col] = SNAKE;
      /* Draw new fruit */
      new_fruit(game_board);
      print_score(snake->length);
      break;
    default:
      /* Catches empty as well */
      /* Update as normal */
      snake_push(snake, next_head_coord);
      /* Add new snake head */
      game_board[next_head_coord.row][next_head_coord.col] = SNAKE;
      break;
  }

  return 1;
}

/**
 * \brief             Show the game start screen
 * \note              Does not call refresh() so multiple prints can happen
 *                    before updating the screen.
 */
void print_game_start() {
  // Total of 3 lines:
  // Welcome  (7)
  //   to     (2)
  //  Snake   (5)

  // Welcome
  int start_row = (screen_rows / 2) - (3 / 2);
  int start_col = (screen_cols / 2) - (7 / 2);
  mvprintw(start_row, start_col, "Welcome");
  start_row++;

  start_col = (screen_cols / 2) - (2 / 2);
  mvprintw(start_row, start_col, "to");
  start_row++;

  start_col = (screen_cols / 2) - (5 / 2);
  mvprintw(start_row, start_col, "Snake");
  start_row++;

  start_row += 5;
  start_col = (screen_cols / 2) - (25 / 2);
  mvprintw(start_row, start_col, "(Press Enter to start...)");

  return;
}

/**
 * \brief             Show the game over screen with the final score
 * \note              Does not call refresh() so multiple prints can happen
 *                    before updating the screen.
 * \param[in,out]     snake: Snake struct from gameplay. Used for length score
 */
void print_game_over(struct Snake *snake) {
  // Center "Game Over" which is 11 characters. We will have 2 lines:
  //    Game Over
  //    Score: 1
  int start_row = (screen_rows / 2) - ( 2 / 2);
  int start_col = (screen_cols / 2) - (11 / 2);

  mvprintw(start_row, start_col, "Game Over");
  mvprintw(start_row + 1, start_col, "Score: %d", snake->length);

  return;
}

/**
 * \brief             Show the game over screen with the final score
 * \note              Does not call refresh() so multiple prints can happen
 *                    before updating the screen.
 * \param[in,out]     snake: Snake struct from gameplay. Used for length score
 */
void print_score(int score) {
  // Same logic as start of print_game_board so score lines up with the top
  // left corner of the game border
  int start_row = (screen_rows / 2) - (MAX_ROWS / 2) - 1; // 1 row above game board
  int start_col = (screen_cols / 2) - (MAX_COLS / 2);     // Same column as game board start

  mvprintw(start_row, start_col, "Score: %d", score);

  return;
}
