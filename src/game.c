#include "game.h"

const int V_LENGTH = 20;
const int H_LENGTH = 35;
int Y_STDSCR_MAX, X_STDSCR_MAX, Y_WIN_MAX, X_WIN_MAX;
WINDOW *win;
const char HEAD_CHAR = '@';
const char SEG_CHAR = 'O';
const char FOOD_CHAR = '*';
const int MOV_INTV = 100; //  10 movements per second
const char TITLE[] = "Snake";
const int SNK_LEN = 2;

void CheckInput(const char ch, snk_state *state) {
  if (ch == 'j' && *state != SNK_UP)
    *state = SNK_DOWN;
  else if (ch == 'k' && *state != SNK_DOWN)
    *state = SNK_UP;
  else if (ch == 'h' && *state != SNK_RIGHT)
    *state = SNK_LEFT;
  else if (ch == 'l' && *state != SNK_LEFT)
    *state = SNK_RIGHT;
  else if (ch == 'q')
    terminate_session("Exited succesfully", 0);
  else
    return;
}

void terminate_session(const char *msg, const int exit_code) {
  delwin(win);
  endwin();
  printf("%s\n", msg);
  exit(exit_code);
}

void move_snk(void_vec *snake_vec, const snk_state dir) {
  for (int i = snake_vec->size - 1; i >= 1; --i) {
    void_set(snake_vec, (coord *)void_get(snake_vec, i - 1), i);
  }
  coord head_pos = *(coord *)void_get(snake_vec, 0);
  if (dir == SNK_DOWN && head_pos.y_pos < Y_WIN_MAX - 2)
    head_pos.y_pos++;
  else if (dir == SNK_UP && head_pos.y_pos > 1)
    head_pos.y_pos--;
  else if (dir == SNK_LEFT && head_pos.x_pos > 1)
    head_pos.x_pos--;
  else if (dir == SNK_RIGHT && head_pos.x_pos < X_WIN_MAX - 2)
    head_pos.x_pos++;
  void_set(snake_vec, &head_pos, 0);
}

void update_scr(const void_vec *snake_pos, const coord food_pos) {
  wclear(win);
  mvwprintw(win, (*(coord *)void_get(snake_pos, 0)).y_pos,
            (*(coord *)void_get(snake_pos, 0)).x_pos, "%c", HEAD_CHAR);
  for (int i = 1; i < snake_pos->size; ++i) {
    mvwprintw(win, (*(coord *)void_get(snake_pos, i)).y_pos,
              (*(coord *)void_get(snake_pos, i)).x_pos, "%c", SEG_CHAR);
  }
  mvwprintw(win, food_pos.y_pos, food_pos.x_pos, "%c", FOOD_CHAR);
  box(win, 0, 0);
  mvwprintw(win, 0, X_WIN_MAX / 2 - strlen(TITLE) / 2, "%s", TITLE);
  wrefresh(win);
}

long long now(void) {
  struct timespec time_now;
  clock_gettime(CLOCK_MONOTONIC, &time_now);
  return time_now.tv_sec * 1000LL + time_now.tv_nsec / 1000000LL;
}

void snake_food_gen(coord *position) {

  position->x_pos = rand() % (X_WIN_MAX - 2) + 1;
  position->y_pos = rand() % (Y_WIN_MAX - 2) + 1;
}

void snake_grow(void_vec *snake_vector) {
  coord second_to_last =
      *(coord *)void_get(snake_vector, snake_vector->size - 2);
  coord last = *(coord *)void_get(snake_vector, snake_vector->size - 1);
  int y_diff = last.y_pos - second_to_last.y_pos;
  int x_diff = last.x_pos - second_to_last.x_pos;
  coord new_pos = {.y_pos = last.y_pos + y_diff, .x_pos = last.x_pos + x_diff};
  void_append(snake_vector, &new_pos);
}

void init_sk_len(void_vec *snake_vector, const int len) {
  coord head_pos = *(coord *)void_get(snake_vector, 0);
  coord new_seg = {.y_pos = head_pos.y_pos, .x_pos = head_pos.x_pos};
  for (int i = 0; i < len - 1; ++i) {
    void_append(snake_vector, &new_seg);
  }
}

bool snk_collided(const void_vec *snk_vec) {
  coord head_pos = *(coord *)void_get(snk_vec, 0);
  for (int i = 1; i < snk_vec->size; ++i) {
    coord seg_pos = *(coord *)void_get(snk_vec, i);
    if (head_pos.y_pos == seg_pos.y_pos && head_pos.x_pos == seg_pos.x_pos)
      return true;
    else if (head_pos.y_pos <= 0 || head_pos.y_pos >= Y_WIN_MAX ||
             head_pos.x_pos <= 0 || head_pos.x_pos >= X_WIN_MAX)
      return true;
  }
  return false;
}
