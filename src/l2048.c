#include "l2048.h"
#include <stdlib.h>

board init(void)
{
    board b;
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
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
            if (b.state[i][j] != '\0') {
                char *c = &b.state[i][j];
                points += atoi(c);
            }
        }
    }
    return points;
}

char merge(char *x, char *y)
{
    int merged = atoi(x) + atoi(y);
    return (char) merged;
}

/*--------  Movement  --------*/
/*
    TODO:
    - Work out general mechanism
    - Implement for every case
    - Test
 */

board move_left(board b)
{
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (b.state[i][j] == '\0' && b.state[i][j + 1] != '\0') {
                
            }
        }
    }
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