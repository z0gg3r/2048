#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "board.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

void draw(board_t *b);
int remap(int ch);

int main()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	time_t t;
	srand((unsigned) time(&t));
	board_t *b = init();
	int stop = game_over(b);
	spawn(b, 1);
	draw(b);
	int ch = getch();
	ch = remap(ch);
	clear();
	switch (ch) {
		case LEFT:
			mv_left(b);
			spawn(b, 0);
			draw(b);
			break;
		case RIGHT:
			mv_right(b);
			spawn(b, 0);
			draw(b);
			break;
		case UP:
			mv_up(b);
			spawn(b, 0);
			draw(b);
			break;
		case DOWN:
			mv_down(b);
			spawn(b, 0);
			draw(b);
			break;
	}
	while (!stop) {
		ch = getch();
		ch = remap(ch);
		clear();
		switch (ch) {
			case LEFT:
				mv_left(b);
				spawn(b, 0);
				draw(b);
				break;
			case RIGHT:
				mv_right(b);
				spawn(b, 0);
				draw(b);
				break;
			case UP:
				mv_up(b);
				spawn(b, 0);
				draw(b);
				break;
			case DOWN:
				mv_down(b);
				spawn(b, 0);
				draw(b);
				break;
		}
		stop = game_over(b);
	}
	clear();
	printw("Game Over!\n Final Points: %d\n", b->points);
	printw("(Press any key to quit)");
	free_board(b);
	ch = getch();
	endwin();
	return 0;
}

void draw(board_t *b)
{
	printw("Points: %d \n", b->points);
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			wprintw(stdscr, " %x ", b->cols[i]->cells[j]);
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
	}
}
