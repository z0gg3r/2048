#include "board.h"

void mv_left(board_t *b)
{
	/*
	 * We take every column and look for every cell for the following conditions:
	 * 	1. Is the entire Column just empty? If yes, do nothing.
	 * 	2. Is the next non-empty cell the same as the current cell? If yes, merge.
	 * 	3. Is the current cell empty and there is a non-empty cell? If yes, move.
	 * 	4. Is the next non-empty cell not the same as the current cell? If yes, do nothing.
	 * If a condition is met for a current cell and we have to move or merge, then we break
	 * and move on to the next cell.
	 *
	 */
	int limit;
	int idx = 0;
	int cell;
	for (int i =  0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			limit = NUM_CELLS - (1 + j;)
			while (idx < limit) {
				cell = j + 1 + idx;
				if (!b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// There is a non-empty cell and the current cell is empty,
					// so move the value and break.
					b->cols[i]->cells[j] = b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
					break;
				} else if (b->cols[i]->cells[cell] == b->cols[i]->cells[j]) {
					// The next non-empty cell and the current one match,
					// so merge the values and break;
					b->cols[i]->cells[j] *= 2;
					b->cols[i]->cells[cell] = 0;
					break;	
				} else {
					// Not really needed, but will stay as a kind of
					// catch all guard against undefined shit.
					++idx;
				}
			}
		}
	}
	calc_points(b);
}

void mv_right(board_t *b)
{
	/*
	 * This is like mv_left, but we move right instead,
	 * so we do the opposite of what we did with mv_left.
	 *
	 */
	int limit;
	int idx = 0;
	int cell;
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = NUM_CELLS - 1; j >= 0; --j) {
			limit = j;
			while (idx < limit) {
				cell = j - 1 - idx;
				if (!b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// There is a non empty cell and the current cell is empty,
					// so move the value and break.
					b->cols[i]->cells[j] = b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
					break;
				} else if (b->cols[i]->cells[cell] == b->cols[i]->cells[j]) {
					// The next non-empty cell and the current one match,
					// so merge the values and break.
					b->cols[i]->cells[j] *= 2;
					b->cols[i]->cells[cell] = 0;
					break;
				} else {
					// Not really needed, but will stay as a kind of
					// catch all guard against undefined shit.
					++idx;
				}
			}
		}
	}
	calc_points(b);
}
