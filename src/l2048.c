#include "l2048.h"
#include <stdlib.h>

board init(void)
{
    board b;
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (i == 0 || j == 0)
                b.state[i][j] = '#';
            else if (j + 1 == GRID_X || i + 1 == GRID_Y)
                b.state[i][j] = '#';
            else
                b.state[i][j] = '\0';
        }
    }
    return b;
}

int calc_points(board b)
{
    int points = 0;
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            char *c = &b.state[i][j];
            if (i == 0 || j == 0)
                points += 0;
            else if (j + 1 == GRID_X || i + 1 == GRID_Y)
                points += 0;
            else if (b.state[i][j] == '\0')
                points += 0;
            else
                points += atoi(c);
        }
    }
    return points;
}

board move_left(board b)
{
    return b;
}

board move_right(board b)
{
    return b;
}

board move_up(board b)
{
    return b;
}

board move_down(board b)
{
    return b;
}