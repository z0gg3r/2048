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
board move(board, int);
int calc_points(board);


#endif // !2048_H