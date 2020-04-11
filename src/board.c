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
			limit = NUM_CELLS - (1 + j);
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
			idx = 0;
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
			idx = 0;
		}
	}
	calc_points(b);
}

void mv_up(board_t *b)
{
	/*
	 * This is just like mv_left, only with
	 * variables swapped.
	 *
	 */
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = 0; j < NUM_ROWS; ++j) {
			limit = NUM_ROWS - (1 + j);
			while (idx < limit) {
				col = j + 1 + idx;
				if (!b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell and the other cell are both zero,
					// Look at the next cell.
					++idx;
				} else if (b->cols[col]->cells[i] && !b->cols[col]->cells[i]) {
					// Current cell is zero, but we have found a
					// non-zero one, move and break. 
					b->cols[j]->cells[i] = b->cols[col]->cells[i];
					b->cols[col]->cells[i] = 0;
					break;
				} else if (b->cols[col]->cells[i] == b->cols[j]->cells[i]) {
					// Current cell and next non-zero one match, merge
					// and break;
					b->cols[j]->cells[i] *= 2;
					b->cols[col]->cells[i] = 0;
					break;
				} else {
					// Catch-All guard thingy
					++idx;
				}
			}
			idx = 0;
		}
	}
	calc_points(b);
}

void mv_down(board_t *b)
{
	/*
	 * This is just like mv_up, only from
	 * the other side, so like mv_right
	 * is to mv_left...
	 *
	 */
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = NUM_ROWS - 1; j >= 0; --j) {
			limit = j;
			while (idx < limit) {
				col = j - 1 - idx;
				if (!b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell and the other one are both
					// zero, so move to the next cell
					++idx;
				} else if (b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell is zero, but we've found a
					// non-zero one, so move and break
					b->cols[j]->cells[i] = b->cols[col]->cells[i];
					b-cols[col]->cells[i] = 0;
					break;
				} else if (b->cols[col]->cells[i] == b->cols[j]->cells[i]) {
					// Current cell and the next non-zero oen
					// match, so merge and break.
					b->cols[j]->cells[i] *= 2;
					b->cols[col]->cells[i] = 0;
					break;
				} else {
					// Catch-All guard thingy
					++idx;
				}
			}
			idx = 0;
		}
	}
	calc_points(b);
}

void calc_points(board_t *b)
{

}
