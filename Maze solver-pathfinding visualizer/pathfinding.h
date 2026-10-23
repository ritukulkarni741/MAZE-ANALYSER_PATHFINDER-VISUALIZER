#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "main.h"

bool solveDFS(const Maze *maze, bool path[MAX_ROWS][MAX_COLS], int *visitedCount);
bool solveBFS(const Maze *maze, bool path[MAX_ROWS][MAX_COLS], int *visitedCount);

#endif