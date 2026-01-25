#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector.h> // THIS IS A CUSTOM EXTERNAL DEPENDENCY CHECK README

typedef enum snk_state {
  SNK_UP,
  SNK_DOWN,
  SNK_LEFT,
  SNK_RIGHT,
  SNK_NAN
} snk_state;

typedef struct coord {
  int x_pos;
  int y_pos;
} coord;

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

/* constant that defines the time interval between the snake movements */

const extern int MOV_INTV; // MILISECONDS

/* Title */

const extern char TITLE[];

/* Expects to get a char input and decides where the character will move until
 * it recieves new input*/

void CheckInput(const char ch, snk_state *state);

/* Recieves a mesage (either error msg or a succesful exit msg), terminates the
 * game and curses window and stdscr prints the mesage in the terminal buffer
 * and ends the program with a recieved exit code*/

void terminate_session(const char *msg, const int exit_code);

/* updates the snake coordinates */

void move_snk(coord *pos, const snk_state dir);

/* Updates the render of the snake in the screen */

void update_scr(const coord pos);

/* Returns a long long representing the number of miliseconds since an Epoch */

long long now(void);

#endif // GAME_H