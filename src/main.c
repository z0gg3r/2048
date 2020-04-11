#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

void draw(board_t *b);

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	board_t b;
	b.points = 0;
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		column_t c;
		for (int j = 0; j < NUM_CELLS; ++j) {
			c.cells[j] = 0;
		}
		b.cols[i] = &c;
	}
	init(&b);
	spawn(&b, 1);
	draw(&b);
	mv_left(&b);
	draw(&b);
}

void draw(board_t *b)
{
	printf("Points: %d \n", b->points);
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			printf(" %d ", b->cols[i]->cells[j]);
		}
		printf("\n");
	}
}
