#ifndef MAIN_H
#define MAIN_H

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define RESET 4
#define QUIT 5

#define SPACING 10

#define EXEC_FAILED 127

#if USE_SCORES_IN_PATH
	const char *SCORES_PATH = "scores";
#else
	const char *SCORES_PATH = "./scores";
#endif

static int FLAG_RESET = 0;
static int FLAG_QUIT = 0;

void draw(board_t *b, WINDOW *w);
WINDOW *init_win();
int remap(int ch);
int game_loop(WINDOW *w);
void write_scores(board_t *b);


#endif
