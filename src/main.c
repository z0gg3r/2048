#include <stdio.h>
#include "board.h"

void draw(board_t *b);

int main()
{
	board_t b;
	init(&b);
	spawn(&b, 1, 1);
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
