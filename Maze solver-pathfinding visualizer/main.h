#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

#define MAX_ROWS 50
#define MAX_COLS 50

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    int rows;
    int cols;
    char cells[MAX_ROWS][MAX_COLS];
    Position start;
    Position end;
} Maze;

bool loadMaze(const char *filename, Maze *maze);
void printMaze(const Maze *maze, bool path[MAX_ROWS][MAX_COLS], const char *title);

#endif
