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

void mv_left(board_t *b)
{
	/*
	 * For every cell in every column we look at the cell (`current cell`) and
	 * a differenct cell (`other cell`) and perform some condition checks to see
	 * how we should behave. They are listed in the order they are performed.
	 * 	0. Are neither the current nor the other cell empty and are they
	 * 	   also both not the same?
	 * 	1. Are the current cell and the other cell empty?
	 * 	2. Is the other cell filled with a value. but the current cell is
	 * 	   empty?
	 * 	3. Are the values of the current and the other cell the same? (We
	 * 	   don't have to check for empty cells here, because we already
	 * 	   know that they are not empty)
	 * 	4. Is anything else true? Like really? Maybe a cell is in some
	 * 	   sort of superposition because we've used threadsanitze.
	 * 
	 * Now we can not react to all conditions in the same manner (if we could
	 * we would not have to check for anything and just do that), so here is
	 * how we respond (ordered according to the numbers up there):
	 * 	0. We move our current cell one up. Why? Because we have a 
	 * 	   current cell that is filled with a value and a diffrent cell
	 * 	   that is also filled with a value, but those aren't the same,
	 * 	   so even if there is a cell with a value somwhere behind that
	 * 	   other cell, we can't reach it because the other cell blocks it.
	 * 	1. Welp,  can't do anything, but move the other cell one up.
	 * 	2. Yes! That is good. We can now move the value of the other cell
	 * 	   into the current cell and then empty the other cell. We can't
	 * 	   break however, since a merger would at this point be still
	 * 	   legal and thus we instead just move `other cell` one up.
	 * 	3. Yes! That is good, again. We have a match in value, the current
	 * 	   cell and that other cell can be merged, so we multiply the
	 * 	   current cell by two, empty the other cell and add the value of
	 * 	   the current cell to the points. We then can move on to the next
	 * 	   cell to be our current cell.
	 * 	4. Since we do not know how and why this evaluated to true, we will
	 * 	   not do anything and act as if they were both empty. We can not
	 * 	   merge case 1 with this, since we have to check if the cells are
	 * 	   both empty, but we might be able to remove this, but it does not
	 * 	   hurt so why should we?
	 *
	 * Since all move functions work based upon this logic, only the difference
	 * in the mathy bit are explained.
	 * 
	 * Maths bit:
	 * limit 	--> How many cells are after the other cell we are currently looking
	 * 		    at. This is calculated as the number of cells minus the original
	 * 		    other cell position.
	 * idx		--> The offset of the other cell from the original other cell (j + 1).
	 * cell		--> The position of the other cell.
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
				if (b->cols[i]->cells[j] && b->cols[i]->cells[cell] && b->cols[i]->cells[cell] != b->cols[i]->cells[j]) {
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

void mv_right(board_t *b)
{
	/*
	 * For the logic see `mv_left`.
	 *
	 * Maths bit:
	 * limit	--> Same as mv_left, but limit is always equal to j.
	 * idx		--> Same as mv_left.
	 * cell		--> Same as left.
	 *
	 * Whereas in mv_left we only had to count up until the limit in both
	 * for loops, we now have to count down from one less then the limit
	 * in the second for loop.
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
				if (b->cols[i]->cells[j] && b->cols[i]->cells[cell] && b->cols[i]->cells[cell] != b->cols[i]->cells[j]) {
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

void mv_up(board_t *b)
{
	/*
	 * For the logic see `mv_left`
	 *
	 * Maths bit:
	 * limit 	--> Same as mv_left
	 * idx 		--> Same as mv_left
	 * cell 	--> Same as mv_left, only named `col`
	 *
	 * While the for loops are theoretically the same, the positions
	 * are swapped, i.e. the outer for loop is for the number of cells
	 * while the inner one is for the columns.
	 */
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = 0; j < NUM_COLUMNS; ++j) {
			limit = NUM_COLUMNS - (1 + j);
			while (idx < limit) {
				col = j + 1 + idx;
				if (b->cols[j]->cells[i] && b->cols[col]->cells[i] && b->cols[col]->cells[i] != b->cols[j]->cells[i]) {
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

void mv_down(board_t *b)
{
	/*
	 * For the logic see `mv_left`
	 *
	 * Maths bit:
	 * limit --> Same as mv_right
	 * idx --> Same as mv_right
	 * cell --> same as mv_right, only called `col`
	 *
	 * Here we have the loop swap of mv_up combined with
	 * the counting swap of mv_right
	 *
	 */
	int limit;
	int idx = 0;
	int col;
	for (int i = 0; i < NUM_CELLS; ++i) {
		for (int j = NUM_COLUMNS - 1; j >= 0; --j) {
			limit = j;
			while (idx < limit) {
				col = j - 1 - idx;
				if (b->cols[j]->cells[i] && b->cols[col]->cells[i] && b->cols[col]->cells[i] != b->cols[j]->cells[i]) {
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
	return n == 16;
}

