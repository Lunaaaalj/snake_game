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

/* Characters that define how the snake and food is rendered  */
const extern char HEAD_CHAR;
const extern char SEG_CHAR;
const extern char FOOD_CHAR;

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

void move_snk(void_vec *snake_vec, const snk_state dir);

/* Updates the render of the snake in the screen */

void update_scr(const void_vec *snake_pos, const coord food_pos);

/* Returns a long long representing the number of miliseconds since an Epoch */

long long now(void);

/* Generate a pseudo-random pair of coordinates for the rendering of snake food
 */

void snake_food_gen(coord *position);

/* Creates a new snake segment to the oposite direction of the last chunk to the
 * next */

void snake_grow(void_vec *snake_vector);

#endif // GAME_H