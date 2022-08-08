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



#ifndef BOARD_H
#define BOARD_H

#define NUM_COLUMNS 4
#define NUM_CELLS 4
#define CALL_LIMIT NUM_CELLS * NUM_COLUMNS

typedef struct column_t {
	int cells[NUM_CELLS];
} column_t;

typedef struct board_t {
	int points;
	column_t *cols[NUM_COLUMNS];
} board_t;

board_t *init();
void free_board(board_t *b);

void mv_left(board_t *b);
void mv_right(board_t *b);
void mv_up(board_t *b);
void mv_down(board_t *b);

int spawn(board_t *b, int callc);
int game_over(board_t *b);
int _game_over(int *cells);

#endif
