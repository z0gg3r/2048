/*
 *   2048 - A ncurses based 2048 clone
 *   Copyright (C) 2021  zockerfreunde03/z0gg3r
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License Version 3 or
 *   later as published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License or the LICENSE file for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "board.h"
#include "main.h"

/*
 * We initalize the setup of ncurses, seed srand and then enter the
 * game loop and loop until the users tells us that they have had
 * enough of the game. We then end the ncurses session and quit.
 */
int main()
{
	// Setup ncurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	WINDOW *w = init_win();

	// Setup rand
	time_t t;
	srand((unsigned)time(&t));

	int return_code = RESET;

	while (return_code == RESET) {
		return_code = game_loop(w);
	}

	endwin();
	delwin(w);

	return 0;
}

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
 */
int game_loop(WINDOW *w)
{
	FLAG_RESET = 0;
	clear();
	board_t *b = init();
	int ch;

	int stop = spawn(b, 0);
	while (!stop) {
		if (spawn(b, 0))
			break;
		draw(b, w);

		do {
			ch = getch();
			ch = remap(ch);
		} while (ch == -1);
		werase(stdscr);
		werase(w);
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

	// Don't write the scores if the user ended the round by quitting
	// or resetting
	if (!FLAG_QUIT && !FLAG_RESET) {
		if (write_score(b->points) == EXECV_FAILURE) {
			printw("Oops, could not add score to scores file!");
		}
	}

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

/*
 * Currently we first print the number of points
 * and then all values. This will change in the
 * future so that all cells are in the same spot
 * all the time and not three to left if the value
 * of a previous cell is greater than one hundred.
 */
void draw(board_t *b, WINDOW *w)
{
	printw("%s v%s\n", PROGNAME, VERSION);
	printw("Points: %d \n", b->points);
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			wprintw(w, "%*d", 16, b->cols[i]->cells[j]);
		}
		wprintw(w, "\n\n\n\n\n\n\n\n");
	}
	refresh();
	wrefresh(w);
}

/*
 * We initalise a new curses window that is 4 times the columns high
 * and 8 times the cells wide. We then create x and y as LINE/COL
 * minus the width and heigt respectivley and half those to center
 * the window. Then we return a pointer to this new window.
 */
WINDOW *init_win()
{
	WINDOW *w;
	// from board.h
	int height = 8 * NUM_COLUMNS;
	int width = 16 * NUM_CELLS;
	// from main.h
	int x = (LINES - height) / 2;
	int y = (COLS - width) / 2;
	w = newwin(height, width, x, y);
	return w;
}

/*
 * This is called every time the user enters an input
 * and returns a quick response code for the appropritate
 * action so that the code that handles the action can
 * look a bit neater!
 * (The values for the return codes are defined in main.h)
 */
int remap(int ch)
{
	switch (ch) {
	case KEY_LEFT:
	case 'h':
	case 'H':
		return LEFT;
	case KEY_RIGHT:
	case 'l':
	case 'L':
		return RIGHT;
	case KEY_UP:
	case 'k':
	case 'K':
		return UP;
	case KEY_DOWN:
	case 'j':
	case 'J':
		return DOWN;
	case 'r':
	case 'R':
		return RESET;
	case 'q':
	case 'Q':
		return QUIT;
	default:
		return -1;
	}
}

/*
 * This function is called once the game_over is reached and the final
 * score of the round is known. It converts the given score to a char *
 * and writes it to the scores file. First it forks itself to then
 * execute /bin/sh SCORES_SCRIPT score, where SCORES_SCRIPT is the
 * location of scores.sh on the system and score is the char * that the
 * argument was converted to.
 *
 * It returns 0 on success and EXECV_FAILURE (or 127) on failure.
 */
int write_score(int score)
{
	int result = 0;
	char *s = to_str(score);

	if (!s)
		goto execv_failure;

	char *argv[] = { "sh", SCORES_SCRIPT, s, NULL };
	pid_t pid = fork();

	if (!pid) {
		execv("/bin/sh", argv);
		result = EXECV_FAILURE;
		goto execv_failure;
	} else {
		waitpid(pid, 0, 0);
	}

execv_failure:
	free(s);
	return result;
}

/*
 * This functions takes an integer and then returns its value as a
 * cstring.
 */
char *to_str(int i)
{
	// If the provided buffer does not have enough space for the
	// formatted string, then the size that would have been needed
	// without \0 is returned.
	int len = snprintf(NULL, 0, "%d", i) + 1;

	// We now malloc enough space for the new string
	char *buf = calloc(len, sizeof(char));

	if (!buf) {
		perror("2048 - write_score - to_str");

		return NULL;
	}

	// Write string to buf
	snprintf(buf, len, "%d", i);

	// And return it
	return buf;
}
