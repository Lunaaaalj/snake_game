#include "game.h"

const int V_LENGTH = 20;
const int H_LENGTH = 35;
int Y_STDSCR_MAX, X_STDSCR_MAX, Y_WIN_MAX, X_WIN_MAX;
WINDOW *win;
const char HEAD_CHAR = '@';
const char BODY_CHAR = '*';
const int MOV_INTV = 100; //  10 movements per second
const char TITLE[] = "Snake";

void CheckInput(const char ch, snk_state *state) {
  switch (ch) {
  case 'j':
    *state = SNK_DOWN;
    return;
  case 'k':
    *state = SNK_UP;
    return;
  case 'h':
    *state = SNK_LEFT;
    return;
  case 'l':
    *state = SNK_RIGHT;
    return;
  case 'q':
    terminate_session("Session terminated\n", 0);
  default:
    return;
  }
}

void terminate_session(const char *msg, const int exit_code) {
  delwin(win);
  endwin();
  printf("%s\n", msg);
  exit(exit_code);
}

void move_snk(coord *pos, const snk_state dir) {
  if (dir == SNK_DOWN && pos->y_pos < Y_WIN_MAX - 2)
    pos->y_pos++;
  else if (dir == SNK_UP && pos->y_pos > 1)
    pos->y_pos--;
  else if (dir == SNK_LEFT && pos->x_pos > 1)
    pos->x_pos--;
  else if (dir == SNK_RIGHT && pos->x_pos < X_WIN_MAX - 2)
    pos->x_pos++;
  else // if the direction is just nan
    return;
}

void update_scr(const coord pos) {
  wclear(win);
  mvwprintw(win, pos.y_pos, pos.x_pos, "%c", HEAD_CHAR);
  box(win, 0, 0);
  mvwprintw(win, 0, X_WIN_MAX / 2 - strlen(TITLE) / 2, "%s", TITLE);
  wrefresh(win);
}

long long now(void) {
  struct timespec time_now;
  clock_gettime(CLOCK_MONOTONIC, &time_now);
  return time_now.tv_sec * 1000LL + time_now.tv_nsec / 1000000LL;
}
