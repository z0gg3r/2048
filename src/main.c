#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "board.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define RESET 4
#define QUIT 5

static int FLAG_RESET = 0;
static int FLAG_QUIT = 0;

void draw();
int remap(int ch);
int game_loop();

int main()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	time_t t;
	srand((unsigned) time(&t));
	int return_code = RESET;
	while (return_code == RESET) {
		return_code = game_loop();
	}
	endwin();
	return 0;
}

int game_loop()
{
	FLAG_RESET = 0;
	clear();
	board_t *b = init();
	int stop = game_over(b);
	int ch;
	spawn(b);
	while (!stop) {
		spawn(b);
		draw(b);
		ch = getch();
		ch = remap(ch);
		clear();
		switch (ch) {
			case LEFT:
				mv_left(b);
				break;
			case RIGHT:
				mv_right(b);
				break;
			case UP:
				mv_up(b);
				break;
			case DOWN:
				mv_down(b);
				break;
			case RESET:
				FLAG_RESET = 1;
				break;
			case QUIT:
				FLAG_QUIT = 1;
				break;
		}
		stop = FLAG_QUIT || FLAG_RESET || game_over(b);
	}
	clear();
	printw("Game Over!\n Final Points: %d\n", b->points);
	free_board(b);
	if (FLAG_RESET) {
		return RESET;
	}
	if (FLAG_QUIT) {
		return QUIT;
	}
	printw("(Press r to start a new game and q to quit.)");
	ch = getch();
	ch = remap(ch);
	while (ch != RESET && ch != QUIT) {
		ch = getch();
		ch = remap(ch);
	}
	return ch;
}

void draw(board_t *b)
{
	printw("Points: %d \n", b->points);
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			wprintw(stdscr, " %d ", b->cols[i]->cells[j]);
		}
		printw("\n");
	}
	refresh();
}

int remap(int ch)
{
	if (ch == KEY_LEFT || ch == 'h' || ch == 'H') {
		return LEFT;
	} else if (ch == KEY_RIGHT || ch == 'l' || ch == 'L') {
		return RIGHT;
	} else if (ch == KEY_UP || ch == 'k' || ch == 'K') {
		return UP;
	} else if (ch == KEY_DOWN || ch == 'j' || ch == 'J') {
		return DOWN;
	} else if (ch == 'r' || ch == 'R') {
		return RESET;
	} else if (ch == 'q' || ch == 'Q') {
		return QUIT;
	}
}
