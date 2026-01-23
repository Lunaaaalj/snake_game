#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <stdio.h>
#include <time.h>
/* Controls the size of the main game window */
extern const int V_LENGTH;
extern const int H_LENGTH;

/* Define the size values of the current standard screen and the main game
 * window */
extern int Y_STDSCR_MAX, X_STDSCR_MAX;
extern int Y_WIN_MAX, X_WIN_MAX;

/* The only window needed, main screen for the game */
extern WINDOW *win;

/* Characters that define how the snake is rendered */
const extern char HEAD_CHAR;
const extern char BODY_CHAR;

#endif // GAME_H