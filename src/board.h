#ifndef BOARD_H
#define BOARD_H

#define NUM_COLUMNS 4
#define NUM_CELLS 4

typedef struct column_t {
	int cells[NUM_CELLS]
} column_t;

typedef struct board_t {
	int points;
	columnt_t *cols[NUM_COLUMNS];
} board_t;

void mv_left(board_t *b);
void mv_right(board_t *b);
void mv_up(board_t *b);
void mv_down(board_t *b);

void calc_points(board_t *b);

#endif