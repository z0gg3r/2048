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
	srand((unsigned) time(&t));


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
	int stop = game_over(b);
	int ch;
	
	spawn(b);
	while (!stop) {
		spawn(b);
		draw(b, w);
		
		// Enter loop at least once and stay in loop
		// if ch is -1 (or an invalid charachter)
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
		int score = write_score(b->points);

		if (score == EXECV_FAILURE) {
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
	printw("Points: %d \n", b->points);
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			wprintw(w, "%*d", 8, b->cols[i]->cells[j]);
		}
		wprintw(w, "\n\n\n\n");
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
	int height = 4 * NUM_COLUMNS;
	int width = 8 * NUM_CELLS;
	// from curses.h
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
 * (The values for the return codes are defined under
 * the header files)
 * NOTE: This is definetley not the most elegant solution for
 * this function, but it works!
 */
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

	return -1;
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
	char *s = to_str(score);
	char *argv[] = { "/bin/sh", SCORES_SCRIPT, s, NULL };
	pid_t pid = fork();

	if (!pid) {
		execv("/bin/sh", argv);
		free(s);
		return EXECV_FAILURE;
	} else {
		waitpid(pid, 0, 0);
	}

	free(s);
	return 0;
}

/*
 * This functions takes an integer and then returns its value as a
 * cstring.
 */
char *to_str(int i)
{
	// This apparently tells us the length of our number as a cstring.
	// We add one to it, for the terminator c:
	int len = snprintf(NULL, 0, "%d", i) + 1;

	// We now malloc enough space for the new cstring
	char *buf = malloc(len);
	
	// Write cstring to buf
	snprintf(buf, len, "%d", i);

	// And return it
	return buf;
}
