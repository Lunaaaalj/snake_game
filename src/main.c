#include "game.c"
#include "game.h"

/* Formated with clang-format using LLVM conventions */

int main(void) {
  srand(now());
  char ch;
  snk_state state = SNK_NAN;
  long long start, end;
  void_vec snake_pos;
  init_void_vector(&snake_pos, 1, sizeof(coord));
  coord food_pos;
  // Curses initialization
  initscr();
  curs_set(0);
  noecho();
  getmaxyx(stdscr, Y_STDSCR_MAX, X_STDSCR_MAX);
  win = newwin(V_LENGTH, H_LENGTH, Y_STDSCR_MAX / 2 - V_LENGTH / 2,
               X_STDSCR_MAX / 2 - H_LENGTH / 2);
  nodelay(win, true);
  getmaxyx(win, Y_WIN_MAX, X_WIN_MAX);
  box(win, 0, 0);
  coord head_coords = {.y_pos = Y_WIN_MAX / 2, .x_pos = X_WIN_MAX / 2};
  void_append(&snake_pos, &head_coords);
  snake_food_gen(&food_pos);
  update_scr(head_pos, food_pos);
  start = now();
  while (true) {
    CheckInput(wgetch(win), &state);
    end = now();
    if ((end - start) >= MOV_INTV && state != SNK_NAN) {
      move_snk(&head_pos, state);
      update_scr(head_pos, food_pos);
      start = end;
    }
  }
  endwin();
  return 0;
}
