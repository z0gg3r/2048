#include "l2048.h"
#include <stdio.h>

int main(void)
{
    board b = init();
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            if (b.state[i][j] != '\0')
                printf("%c\n", b.state[i][j]);
        }
    }
}