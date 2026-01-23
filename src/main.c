#include "game.c"
#include "game.h"

/* Formated with clang-format using LLVM conventions */

int main(void) {
  char ch;
  // Curses initialization
  initscr();
  curs_set(0);
  noecho();

  getmaxyx(stdscr, Y_STDSCR_MAX, X_STDSCR_MAX);
  win = newwin(V_LENGTH, H_LENGTH, Y_STDSCR_MAX / 2 - V_LENGTH / 2,
               X_STDSCR_MAX / 2 - H_LENGTH / 2);
  getmaxyx(win, Y_WIN_MAX, X_WIN_MAX);
  box(win, 0, 0);
  mvwprintw(win, Y_WIN_MAX / 2, X_WIN_MAX / 2, "%c", HEAD_CHAR);

  endwin();
  return 0;
}