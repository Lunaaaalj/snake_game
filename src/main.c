#include "game.c"
#include "game.h"

/* Formated with clang-format using LLVM conventions */

int main(void) {
  srand(now());
  char ch;
  snk_state state = SNK_NAN;
  long long start, end;
  coord food_pos;
  // Curses initialization
  initscr();
  curs_set(0);
  noecho();
  getmaxyx(stdscr, Y_STDSCR_MAX, X_STDSCR_MAX);

  /* Check if terminal is large enough at startup */
  if (Y_STDSCR_MAX < MIN_TERMINAL_HEIGHT ||
      X_STDSCR_MAX < MIN_TERMINAL_WIDTH) {
    endwin();
    printf("Error: Terminal size too small. Minimum required: %d columns x %d "
           "rows\n",
           MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
    printf("Current size: %d columns x %d rows\n", X_STDSCR_MAX, Y_STDSCR_MAX);
    return 1;
  }

  void_vec snake_pos;
  init_void_vector(&snake_pos, 4, sizeof(coord));
  win = newwin(V_LENGTH, H_LENGTH, Y_STDSCR_MAX / 2 - V_LENGTH / 2,
               X_STDSCR_MAX / 2 - H_LENGTH / 2);
  nodelay(win, true);
  keypad(win, true);
  getmaxyx(win, Y_WIN_MAX, X_WIN_MAX);
  box(win, 0, 0);
  coord head_coords = {.y_pos = Y_WIN_MAX / 2, .x_pos = X_WIN_MAX / 2};
  void_append(&snake_pos, &head_coords);
  snake_food_gen(&food_pos, &snake_pos);
  update_scr(&snake_pos, food_pos);
  init_sk_len(&snake_pos, SNK_LEN);
  start = now();
  while (true) {
    CheckInput(wgetch(win), &state);
    end = now();
    if ((end - start) >= MOV_INTV && state != SNK_NAN) {
      move_snk(&snake_pos, state);
      if ((*(coord *)void_get(&snake_pos, 0)).y_pos == food_pos.y_pos &&
          (*(coord *)void_get(&snake_pos, 0)).x_pos == food_pos.x_pos) {
        snake_grow(&snake_pos);
        snake_food_gen(&food_pos, &snake_pos);
      }
      if (snk_collided(&snake_pos)) {
        HEAD_CHAR = 'X';
        SEG_CHAR = 'X';
        update_scr(&snake_pos, food_pos);
        while ((ch = wgetch(win)) != 'q') {
          if (ch == KEY_RESIZE) {
            if (!handle_resize()) {
              terminate_session(
                  "Error: Terminal size too small. Minimum required: 37 "
                  "columns x 22 rows",
                  1);
            }
            update_scr(&snake_pos, food_pos);
          } else if (ch == KEY_ENTER || ch == '\n') {
            free_void_vector(&snake_pos);
            init_void_vector(&snake_pos, 4, sizeof(coord));
            coord head_coords = {.y_pos = Y_WIN_MAX / 2,
                                 .x_pos = X_WIN_MAX / 2};
            void_append(&snake_pos, &head_coords);
            snake_food_gen(&food_pos, &snake_pos);
            HEAD_CHAR = '#';
            SEG_CHAR = '#';
            update_scr(&snake_pos, food_pos);
            init_sk_len(&snake_pos, SNK_LEN);
            state = SNK_NAN;
            break;
          }
        }
        if (ch == 'q')
          terminate_session("Session Terminated", 0);
      }
      update_scr(&snake_pos, food_pos);
      start = end;
    }
  }
  endwin();
  return 0;
}
