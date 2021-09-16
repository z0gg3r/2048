#include <stdlib.h>
#include "board.h"

/*
 * We allocate space for the board and all
 * the columns and return the board as a
 * pointer.
 */

board_t *init()
{
	board_t *b = malloc(sizeof(board_t));
	b->points = 0;
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		column_t *c = malloc(sizeof(column_t));
		for (int j = 0; j < NUM_CELLS; ++j) {
			c->cells[j] = 0;
		}
		b->cols[i] = c;
	}
	return b;
}

/*
 * We free all the previously allocated
 * space, because it is not cleared when
 * the main thread returns.
 */

void free_board(board_t *b)
{
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		free(b->cols[i]);
	}
	free(b);
}

/*
 * For the logic see MOVE_LOGIC in the repository root.
 * Move all tiles to left and combine if tiles of equal value collide.
 */
void mv_left(board_t *b)
{
	int limit;
	int idx = 0;
	int cell;
	for (int i =  0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			limit = NUM_CELLS - (1 + j);
			while (idx < limit) {
				cell = j + 1 + idx;
				if (b->cols[i]->cells[j] 
					&& b->cols[i]->cells[cell] 
					&& b->cols[i]->cells[cell] != b->cols[i]->cells[j])
				{
					// There might be merge candidates down the line but
					// they are blocked and thus we break.
					break;
				} else if (!b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// There is a non-empty cell and the current cell is empty,
					// so move the value and continue to look, cause there
					// might be a merge candiate further down.
					b->cols[i]->cells[j] = b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
				} else if (b->cols[i]->cells[cell] == b->cols[i]->cells[j]) {
					// The next non-empty cell and the current one match,
					// so merge the values and break;
					b->cols[i]->cells[j] *= 2;
					b->cols[i]->cells[cell] = 0;
					b->points += b->cols[i]->cells[j];
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
}


/*
 * For the logic see MOVE_LOGIC in the repository root.
 * Move all tiles to the right and combine if tiles of equal value collide.
 */
void mv_right(board_t *b)
{
	int limit;
	int idx = 0;
	int cell;
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = NUM_CELLS - 1; j >= 0; --j) {
			limit = j;
			while (idx < limit) {
				cell = j - 1 - idx;
				if (b->cols[i]->cells[j] 
					&& b->cols[i]->cells[cell] 
					&& b->cols[i]->cells[cell] != b->cols[i]->cells[j])
				{
					// There might be merge candidates down the line, but
					// they are blocked, thus we break.
					break;
				} else if (!b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] && !b->cols[i]->cells[j]) {
					// There is a non empty cell and the current cell is empty,
					// so move the value and continue, as there might be
					// further merge candidates down the line.
					b->cols[i]->cells[j] = b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
				} else if (b->cols[i]->cells[cell] == b->cols[i]->cells[j]) {
					// The next non-empty cell and the current one match,
					// so merge the values and break.
					b->cols[i]->cells[j] *= 2;
					b->cols[i]->cells[cell] = 0;
					b->points += b->cols[i]->cells[j];
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
}


/*
 * For the logic see MOVE_LOGIC in the repository root.
 * Move all tiles up and combine if tiles of equal value collide.
 */
void mv_up(board_t *b)
{
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = 0; j < NUM_COLUMNS; ++j) {
			limit = NUM_COLUMNS - (1 + j);
			while (idx < limit) {
				col = j + 1 + idx;
				if (b->cols[j]->cells[i] 
					&& b->cols[col]->cells[i] 
					&& b->cols[col]->cells[i] != b->cols[j]->cells[i])
				{
					// There might be merge candidates down the line,
					// but they are blocked and thus we break.
					break;
				} else if (!b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell and the other cell are both zero,
					// Look at the next cell.
					++idx;
				} else if (b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell is zero, but we have found a
					// non-zero one, move and continue as there
					// might be further merge candidates down
					// the line.
					b->cols[j]->cells[i] = b->cols[col]->cells[i];
					b->cols[col]->cells[i] = 0;
				} else if (b->cols[col]->cells[i] == b->cols[j]->cells[i]) {
					// Current cell and next non-zero one match, merge
					// and break;
					b->cols[j]->cells[i] *= 2;
					b->cols[col]->cells[i] = 0;
					b->points += b->cols[j]->cells[i];
					break;
				} else {
					// Catch-All guard thingy
					++idx;
				}
			}
			idx = 0;
		}
	}
}

/*
 * For the logic see MOVE_LOGIC in the repository root.
 * Move all tiles down and combine if tiles of equal value collide.
 */
void mv_down(board_t *b)
{
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = NUM_COLUMNS - 1; j >= 0; --j) {
			limit = j;
			while (idx < limit) {
				col = j - 1 - idx;
				if (b->cols[j]->cells[i]
					&& b->cols[col]->cells[i] 
					&& b->cols[col]->cells[i] != b->cols[j]->cells[i]) 
				{
					// There might be merge candidates down
					// the line, but they are blocked, thus
					// we break
					break;
				} else if (!b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell and the other one are both
					// zero, so move to the next cell
					++idx;
				} else if (b->cols[col]->cells[i] && !b->cols[j]->cells[i]) {
					// Current cell is zero, but we've found a
					// non-zero one, so move and continue as
					// there might be further merge candidates
					// down the line.
					b->cols[j]->cells[i] = b->cols[col]->cells[i];
					b->cols[col]->cells[i] = 0;
					} else if (b->cols[col]->cells[i] == b->cols[j]->cells[i]) {
					// Current cell and the next non-zero oen
					// match, so merge and break.
					b->cols[j]->cells[i] *= 2;
					b->cols[col]->cells[i] = 0;
					b->points += b->cols[j]->cells[i];
					break;
				} else {
					// Catch-All guard thingy
					++idx;
				}
			}
			idx = 0;
		}
	}
}

/*
 * Here we spawn new tiles on
 * the board. They can be either
 * 2 or 4 in value and only one
 * is spawned at a time.
 */
void spawn(board_t *b)
{
	// Generate a random position for
	// the new tile
	int col = rand() % NUM_COLUMNS;
	int cell = rand() % NUM_CELLS;
	// The original had a 10% chance
	// for the new tile to be a four,
	// instead of a 2, so we try to
	// mimick that here.
	int four = (rand() % 100);
	// Here we simply set the variables
	// in the position, if that's already
	// set then we just call spawn again
	
	if (four <= 10 && !b->cols[col]->cells[cell]) {
		b->cols[col]->cells[cell] = 4;
	} else if (!b->cols[col]->cells[cell]) {
		b->cols[col]->cells[cell] = 2;
	} else {
		spawn(b);
	}
}

/*
 * We count all non-zeros cells and check if they are equal to NUM_COLUMNS
 * times NUM_CELLS (or all cells) and return that.
 */
int game_over(board_t *b)
{
	int n = 0;
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			if (b->cols[i]->cells[j])
				++n;
		}
	}
	return n == (NUM_COLUMNS * NUM_CELLS);
}

