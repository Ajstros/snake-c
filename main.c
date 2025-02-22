#include <curses.h>
#include <ncurses.h>

int getKeyPress();

int main(int argc, char *argv[]) {
  int ch;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  /*1. Press enter to start game*/
  while (getch() != '\n');
  /*2. Show snake*/
  printw("snake");
  refresh();
  getch();
  /*3. Generate fruit*/
  /*4. Move snake according to arrow keys or WASD*/
  /*5. Check for snake collision with fruit, edges of screen, itself*/

  endwin();
  return 0;
}

int getKeyPress() {

  return 0;
}
