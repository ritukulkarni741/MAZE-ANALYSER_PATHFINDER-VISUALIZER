#include "pathfinding.h"
#include "queue.h"
#include <stddef.h>

static const int rowChange[4] = {-1, 1, 0, 0};
static const int colChange[4] = {0, 0, -1, 1};

static bool isInside(const Maze *maze, int row, int col)
{
    return row >= 0 && row < maze->rows &&
           col >= 0 && col < maze->cols;
}

static void clearPath(bool path[MAX_ROWS][MAX_COLS])
{
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            path[row][col] = false;
        }
    }
}

static bool dfsVisit(const Maze *maze, Position current,
                     bool visited[MAX_ROWS][MAX_COLS],
                     bool path[MAX_ROWS][MAX_COLS],
                     int *visitedCount)
{
    if (!isInside(maze, current.row, current.col))
        return false;

    if (maze->cells[current.row][current.col] == '#')
        return false;

    if (visited[current.row][current.col])
        return false;

    visited[current.row][current.col] = true;
    (*visitedCount)++;

    if (current.row == maze->end.row &&
        current.col == maze->end.col) {
        path[current.row][current.col] = true;
        return true;
    }

    for (int direction = 0; direction < 4; direction++) {
        Position next = {
            current.row + rowChange[direction],
            current.col + colChange[direction]
        };

        if (dfsVisit(maze, next, visited, path, visitedCount)) {
            path[current.row][current.col] = true;
            return true;
        }
    }

    return false;
}

bool solveDFS(const Maze *maze,
              bool path[MAX_ROWS][MAX_COLS],
              int *visitedCount)
{
    if (maze == NULL || path == NULL || visitedCount == NULL)
        return false;

    clearPath(path);
    *visitedCount = 0;

    bool visited[MAX_ROWS][MAX_COLS] = {{false}};

    return dfsVisit(maze, maze->start, visited, path, visitedCount);
}

bool solveBFS(const Maze *maze,
              bool path[MAX_ROWS][MAX_COLS],
              int *visitedCount)
{
    if (maze == NULL || path == NULL || visitedCount == NULL)
        return false;

    clearPath(path);
    *visitedCount = 0;

    bool visited[MAX_ROWS][MAX_COLS] = {{false}};
    bool hasParent[MAX_ROWS][MAX_COLS] = {{false}};

    Position parent[MAX_ROWS][MAX_COLS];
    Queue queue;
    Position current;
    bool found = false;

    initQueue(&queue);

    if (!enqueue(&queue, maze->start))
        return false;

    visited[maze->start.row][maze->start.col] = true;
    (*visitedCount)++;

    while (!isQueueEmpty(&queue)) {
        if (!dequeue(&queue, &current))
            break;

        if (current.row == maze->end.row &&
            current.col == maze->end.col) {
            found = true;
            break;
        }

        for (int direction = 0; direction < 4; direction++) {
            Position next = {
                current.row + rowChange[direction],
                current.col + colChange[direction]
            };

            if (!isInside(maze, next.row, next.col))
                continue;

            if (maze->cells[next.row][next.col] == '#')
                continue;

            if (visited[next.row][next.col])
                continue;

            visited[next.row][next.col] = true;
            (*visitedCount)++;

            parent[next.row][next.col] = current;
            hasParent[next.row][next.col] = true;

            if (!enqueue(&queue, next))
                return false;
        }
    }

    if (!found)
        return false;

    Position step = maze->end;

    while (step.row != maze->start.row ||
           step.col != maze->start.col) {
        path[step.row][step.col] = true;

        if (!hasParent[step.row][step.col]) {
            clearPath(path);
            return false;
        }

        step = parent[step.row][step.col];
    }

    path[maze->start.row][maze->start.col] = true;

    return true;
}