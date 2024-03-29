/*
 *   2048 - A ncurses based 2048 clone
 *   Copyright (C) 2021  zockerfreunde03/z0gg3r
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License Version 3 or
 *   later as published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License or the LICENSE file for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/*
 * We allocate space for the board and all
 * the columns and return the board as a
 * pointer.
 */

board_t *init()
{
	board_t *b = calloc(1, sizeof(board_t));

	if (!b) {
		perror("2048: board_t calloc");
		return NULL;
	}

	b->points = 0;
	int i = 0;
	for (i = 0; i < NUM_COLUMNS; ++i) {
		column_t *c = calloc(1, sizeof(column_t));

		if (!c) {
			perror("2048: column_t calloc");

			if (!i) {
				free(b);
				return NULL;
			} else {
				goto abort_column;
			}
		}

		for (int j = 0; j < NUM_CELLS; ++j) {
			c->cells[j] = 0;
		}

		b->cols[i] = c;
	}

	return b;

abort_column:
	for (int j = 0; j < i; ++j) {
		free(b->cols[j]);
	}

	free(b);

	return NULL;
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
	for (int i = 0; i < NUM_COLUMNS; ++i) {
		for (int j = 0; j < NUM_CELLS; ++j) {
			limit = NUM_CELLS - (1 + j);
			while (idx < limit) {
				cell = j + 1 + idx;
				if (b->cols[i]->cells[j] &&
				    b->cols[i]->cells[cell] &&
				    b->cols[i]->cells[cell] !=
					    b->cols[i]->cells[j]) {
					// There might be merge candidates down the line but
					// they are blocked and thus we break.
					break;
				} else if (!b->cols[i]->cells[cell] &&
					   !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] &&
					   !b->cols[i]->cells[j]) {
					// There is a non-empty cell and the current cell is empty,
					// so move the value and continue to look, cause there
					// might be a merge candiate further down.
					b->cols[i]->cells[j] =
						b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
				} else if (b->cols[i]->cells[cell] ==
					   b->cols[i]->cells[j]) {
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
				if (b->cols[i]->cells[j] &&
				    b->cols[i]->cells[cell] &&
				    b->cols[i]->cells[cell] !=
					    b->cols[i]->cells[j]) {
					// There might be merge candidates down the line, but
					// they are blocked, thus we break.
					break;
				} else if (!b->cols[i]->cells[cell] &&
					   !b->cols[i]->cells[j]) {
					// Current cell and the other cell we are looking at
					// are both empty. Increment idx and look at the next
					// cell.
					++idx;
				} else if (b->cols[i]->cells[cell] &&
					   !b->cols[i]->cells[j]) {
					// There is a non empty cell and the current cell is empty,
					// so move the value and continue, as there might be
					// further merge candidates down the line.
					b->cols[i]->cells[j] =
						b->cols[i]->cells[cell];
					b->cols[i]->cells[cell] = 0;
				} else if (b->cols[i]->cells[cell] ==
					   b->cols[i]->cells[j]) {
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
				if (b->cols[j]->cells[i] &&
				    b->cols[col]->cells[i] &&
				    b->cols[col]->cells[i] !=
					    b->cols[j]->cells[i]) {
					// There might be merge candidates down the line,
					// but they are blocked and thus we break.
					break;
				} else if (!b->cols[col]->cells[i] &&
					   !b->cols[j]->cells[i]) {
					// Current cell and the other cell are both zero,
					// Look at the next cell.
					++idx;
				} else if (b->cols[col]->cells[i] &&
					   !b->cols[j]->cells[i]) {
					// Current cell is zero, but we have found a
					// non-zero one, move and continue as there
					// might be further merge candidates down
					// the line.
					b->cols[j]->cells[i] =
						b->cols[col]->cells[i];
					b->cols[col]->cells[i] = 0;
				} else if (b->cols[col]->cells[i] ==
					   b->cols[j]->cells[i]) {
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
				if (b->cols[j]->cells[i] &&
				    b->cols[col]->cells[i] &&
				    b->cols[col]->cells[i] !=
					    b->cols[j]->cells[i]) {
					// There might be merge candidates down
					// the line, but they are blocked, thus
					// we break
					break;
				} else if (!b->cols[col]->cells[i] &&
					   !b->cols[j]->cells[i]) {
					// Current cell and the other one are both
					// zero, so move to the next cell
					++idx;
				} else if (b->cols[col]->cells[i] &&
					   !b->cols[j]->cells[i]) {
					// Current cell is zero, but we've found a
					// non-zero one, so move and continue as
					// there might be further merge candidates
					// down the line.
					b->cols[j]->cells[i] =
						b->cols[col]->cells[i];
					b->cols[col]->cells[i] = 0;
				} else if (b->cols[col]->cells[i] ==
					   b->cols[j]->cells[i]) {
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
 * Here we spawn new tiles on the board. They can be either
 * 2 or 4 in value and only one is spawned at a time. If we
 * have to call spawn again, then we increase the call count
 * and we break once that reaches CALL_LIMIT (i.e. game_over)
 * to avoid softlocking.
 */
int spawn(board_t *b, int callc)
{
	if (callc == CALL_LIMIT)
		return 1;
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

	if (four < 10 && !b->cols[col]->cells[cell]) {
		b->cols[col]->cells[cell] = 4;
	} else if (!b->cols[col]->cells[cell]) {
		b->cols[col]->cells[cell] = 2;
	} else {
		return spawn(b, ++callc);
	}

	return 0;
}

/*
 * We count all non-zeros cells and check if they are equal to NUM_COLUMNS
 * times NUM_CELLS (or all cells) and return that.
 */
int game_over(board_t *b)
{
	int n = 0;
	for (int i = 0; i < NUM_COLUMNS; ++i)
		n += _game_over(b->cols[i]->cells);

	return n == (NUM_COLUMNS * NUM_CELLS);
}

/*
 * Helper to game_over
 */
int _game_over(int *cells)
{
	int n = 0;
	for (int i = 0; i < NUM_CELLS; ++i) {
		if (cells[i]) {
			++n;
		}
	}

	return n;
}
