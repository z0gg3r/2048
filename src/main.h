#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include "board.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define RESET 4
#define QUIT 5

#define SPACING 10

#define EXEC_FAILED 127

#define SCORES_SCRIPT "scores.sh"

static int FLAG_RESET = 0;
static int FLAG_QUIT = 0;

void draw(board_t *b, WINDOW *w);
WINDOW *init_win();
int remap(int ch);
int game_loop(WINDOW *w);
void write_scores(board_t *b);
char *to_str(int i);

#endif
