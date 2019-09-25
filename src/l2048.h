#ifndef L2048_H
#define L2048_H

#define GRID_X 6
#define GRID_Y 6
#define END_OF_GAME 2048

typedef struct board {
    char state[GRID_Y][GRID_X];
} board;

#define MERGE(x) x*2

board init(void);
board move_left(board);
board move_right(board);
board move_up(board);
board move_down(board);
int calc_points(board);


#endif // !L2048_H