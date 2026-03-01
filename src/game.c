#include "game.h"

const int V_LENGTH = 20;
const int H_LENGTH = 35;
const int MIN_TERMINAL_HEIGHT = 22; // V_LENGTH + 2 for safety
const int MIN_TERMINAL_WIDTH = 37;  // H_LENGTH + 2 for safety
int Y_STDSCR_MAX, X_STDSCR_MAX, Y_WIN_MAX, X_WIN_MAX;
WINDOW *win;
char HEAD_CHAR = '#';
char SEG_CHAR = '#';
const char FOOD_CHAR = '*';
const int MOV_INTV = 100; //  10 movements per second
const char TITLE[] = "Snake";
const int SNK_LEN = 2;
const char PATH[] = SNAKE_DATA_DIR "/high_score.txt";

bool CheckInput(const int ch, snk_state *state,snk_state *requested) {
  if ((ch == 'j' || ch == KEY_DOWN) && *state != SNK_UP)
    *requested = SNK_DOWN;
  else if ((ch == 'k' || ch == KEY_UP) && *state != SNK_DOWN)
    *requested = SNK_UP;
  else if ((ch == 'h' || ch == KEY_LEFT) && *state != SNK_RIGHT)
    *requested = SNK_LEFT;
  else if ((ch == 'l' || ch == KEY_RIGHT) && *state != SNK_LEFT)
    *requested = SNK_RIGHT;
  else if (ch == 'q')
    terminate_session("Exited successfully", 0);
  else if (ch == KEY_RESIZE) {
    if (!handle_resize()) {
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg),
               "Error: Terminal size too small. Minimum required: %d columns x "
               "%d rows",
               MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
      terminate_session(error_msg, 1);
    }
    return true; /* Indicate that a resize was handled */
  }
  return false;
}

void terminate_session(const char *msg, const int exit_code) {
  delwin(win);
  endwin();
  printf("%s\n", msg);
  exit(exit_code);
}

void move_snk(void_vec *snake_vec, const snk_state dir, snk_state *state) {
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
  *state = dir;
}

void update_scr(const void_vec *snake_pos, const coord food_pos, const int score) {
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
  char str[] = "Score:";
  mvwprintw(win, Y_WIN_MAX-1, X_WIN_MAX /2 - (strlen(str) + 4) / 2, "%s %3d", str, score);
  wrefresh(win);
}

long long now(void) {
  struct timespec time_now;
  clock_gettime(CLOCK_MONOTONIC, &time_now);
  return time_now.tv_sec * 1000LL + time_now.tv_nsec / 1000000LL;
}

void snake_food_gen(coord *position, const void_vec *snake_pos) {
  coord seg_pos;
  bool collision = false;
  while (true) {
    position->x_pos = rand() % (X_WIN_MAX - 2) + 1;
    position->y_pos = rand() % (Y_WIN_MAX - 2) + 1;
    for (int i = 0; i < snake_pos->size; ++i) {
      seg_pos = *(coord *)void_get(snake_pos, i);
      if (position->x_pos == seg_pos.x_pos &&
          position->y_pos == seg_pos.y_pos) {
        collision = true;
        break;
      } else
        collision = false;
    }
    if (!collision)
      break;
  }
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

  /* Check wall collision once for the head position */
  if (head_pos.y_pos <= 0 || head_pos.y_pos >= Y_WIN_MAX ||
      head_pos.x_pos <= 0 || head_pos.x_pos >= X_WIN_MAX) {
    return true;
  }

  /* Check collision with snake body segments */
  for (int i = 1; i < snk_vec->size; ++i) {
    coord seg_pos = *(coord *)void_get(snk_vec, i);
    if (head_pos.y_pos == seg_pos.y_pos && head_pos.x_pos == seg_pos.x_pos)
      return true;
  }
  return false;
}

bool handle_resize(void) {
  /* Get the new terminal dimensions */
  getmaxyx(stdscr, Y_STDSCR_MAX, X_STDSCR_MAX);

  /* Check if terminal is large enough */
  if (Y_STDSCR_MAX < MIN_TERMINAL_HEIGHT ||
      X_STDSCR_MAX < MIN_TERMINAL_WIDTH) {
    return false;
  }

  /* Delete the old window and create a new one centered in the resized terminal
   */
  delwin(win);
  win = newwin(V_LENGTH, H_LENGTH, Y_STDSCR_MAX / 2 - V_LENGTH / 2,
               X_STDSCR_MAX / 2 - H_LENGTH / 2);
  nodelay(win, true);
  keypad(win, true);

  /* Update window dimensions */
  getmaxyx(win, Y_WIN_MAX, X_WIN_MAX);

  /* Refresh the standard screen and the window */
  refresh();

  return true;
}

/* get the highest score in a path */
int get_high_score(const char *path) {
    FILE *file;
    int score;
    file = fopen(path,"r");
    if (!file) terminate_session("File not found",1);
    fscanf(file,"%d",&score);
    fclose(file);
    return score;
}

/* write the highest score in a path */

void write_high_score(const int score ,const char *path) {
    FILE *file;
    file = fopen(path, "w");
    if (!file) terminate_session("File not found",1);
    fprintf(file, "%d", score);
    fclose(file);
    return;
}

/* display the hightest score */
void disp_hscore(const int high_score) {
    char str[] = "New Highest Score:";
    mvwprintw(win,Y_WIN_MAX - 1,X_WIN_MAX /2 - (strlen(str) + 4) / 2,"%s %3d",str, high_score);
    wrefresh(win);
}
