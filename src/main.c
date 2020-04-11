#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

void draw(board_t *b);

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	board_t *b = init();
	printf("Inital\n");
	draw(b);
	spawn(b, 1);
	printf("First spawn\n");
	draw(b);
	mv_left(b);
	printf("Moved left\n");
	draw(b);
	spawn(b, 0);
	printf("Next spawn\n");
	draw(b);
	mv_down(b);
	printf("Moved down\n");
	draw(b);
	spawn(b, 0);
	printf("Next spawn\n");
	draw(b);
	mv_up(b);
	printf("Moved up\n");
	draw(b);
	spawn(b, 0);
	printf("Next spawn\n");
	draw(b);
	mv_right(b);
	printf("Moved right\n");
	draw(b);
	free_board(b);
	return 0;
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
