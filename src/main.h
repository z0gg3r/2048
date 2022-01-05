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



#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#include "board.h"

#define PROGNAME "2048"
#define VERSION "0.2.1"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define RESET 4
#define QUIT 5

#define SPACING 10

#define EXECV_FAILURE 127

#define SCORES_SCRIPT "/usr/local/share/2048_scores.sh"

static int FLAG_RESET = 0;
static int FLAG_QUIT = 0;

void draw(board_t *b, WINDOW *w);
WINDOW *init_win();
int remap(int ch);
int game_loop(WINDOW *w);
int write_score(int score);
char *to_str(int i);

#endif
