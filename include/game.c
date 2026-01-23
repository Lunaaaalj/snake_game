#include "game.h"

const int V_LENGTH = 20;
const int H_LENGTH = 35;
int Y_STDSCR_MAX, X_STDSCR_MAX, Y_WIN_MAX, X_WIN_MAX;
WINDOW *win;
const char HEAD_CHAR = '@';
const char BODY_CHAR = '*';