#ifndef L2048_H
#define L2048_H

/*
/ GRID_X and GRID_Y should be 2 bigger then the actual playing field
*/
#define GRID_X 4
#define GRID_Y 4
#define END_OF_GAME 2048

typedef struct board {
    char state[GRID_Y][GRID_X];
} board;

board init(void);
board move_left(board);
board move_right(board);
board move_up(board);
board move_down(board);
int calc_points(board);
char merge(char*, char*);

#endif // !L2048_H