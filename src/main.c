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
	/*
	 * We initalize the setup of ncurses, seed srand and then enter the
	 * game loop and loop until the users tells us that they have had
	 * enough of the game. We then end the ncurses session and quit.
	 *
	 */
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
	/*
	 * We first unset the reset flag (so that we can actually play
	 * multiple times) and then clear the screen. We create a new
	 * board and spawn the first to values. We then draw the current
	 * board and wait for user input to move in that direction, reset
	 * or quit. If after the switch either reset, quit or game_over
	 * are true, we exit the while loop, if not we repeat the last
	 * sentence. When the while loop exits, we print a message with
	 * the final points, call free_board to deallocate memory and
	 * then either return reset, quit or wait for user input.
	 * If we wait for user input, we wait until the user enters
	 * either `r` to start a new game or `q` to quit. We then return
	 * what the user entered.
	 *
	 */
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
	/*
	 * Currently we first print the number of points
	 * and then all values. This will change in the
	 * future so that all cells are in the same spot
	 * all the time and not three to left if the value
	 * of a previous cell is greater than one hundred.
	 *
	 */
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
	/*
	 * This is called every time the user enters an input
	 * and returns a quick response code for the appropritate
	 * action so that the code that handles the action can
	 * look a bit neater!
	 * (The values for the return codes are defined under
	 * the header files)
	 */
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
