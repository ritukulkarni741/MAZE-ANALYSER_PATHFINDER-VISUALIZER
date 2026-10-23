
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ROWS 13
#define COLS 21
#define CELL_SIZE 35
#define TOP_BAR 70
#define MAX_CELLS (ROWS * COLS)

typedef struct {
    int row;
    int col;
} Cell;

static int maze[ROWS][COLS];
static bool visited[ROWS][COLS];
static Cell parent[MAX_CELLS];
static Cell queue[MAX_CELLS];
static Cell path[MAX_CELLS];

static int startRow = 1, startCol = 1;
static int goalRow = 11, goalCol = 19;

static int front = 0, rear = 0;
static int pathLength = 0;
static int pathIndex = 0;

static bool searching = false;
static bool finished = false;
static bool found = false;
static float timer = 0.0f;

static const int dr[] = {-1, 1, 0, 0};
static const int dc[] = {0, 0, -1, 1};

static int toIndex(int row, int col) {
    return row * COLS + col;
}

static void createMaze(void) {
    // Begin with an open grid.
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            maze[r][c] = 0;
        }
    }

    // Create boundary walls.
    for (int c = 0; c < COLS; c++) {
        maze[0][c] = 1;
        maze[ROWS - 1][c] = 1;
    }

    for (int r = 0; r < ROWS; r++) {
        maze[r][0] = 1;
        maze[r][COLS - 1] = 1;
    }

    // Internal walls with gaps for navigation.
    for (int r = 1; r <= 9; r++) {
        if (r != 3) maze[r][5] = 1;
    }

    for (int r = 3; r <= 11; r++) {
        if (r != 7) maze[r][10] = 1;
    }

    for (int r = 1; r <= 9; r++) {
        if (r != 5) maze[r][15] = 1;
    }

    for (int c = 6; c <= 14; c++) {
        if (c != 8) maze[4][c] = 1;
    }

    for (int c = 1; c <= 4; c++) {
        maze[8][c] = 1;
    }

    for (int c = 11; c <= 19; c++) {
        if (c != 16) maze[8][c] = 1;
    }

    maze[startRow][startCol] = 0;
    maze[goalRow][goalCol] = 0;
}

static void resetSearch(void) {
    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < MAX_CELLS; i++) {
        parent[i] = (Cell){-1, -1};
    }

    front = 0;
    rear = 0;
    pathLength = 0;
    pathIndex = 0;

    searching = false;
    finished = false;
    found = false;
    timer = 0.0f;
}

static void startBFS(void) {
    resetSearch();

    queue[rear++] = (Cell){startRow, startCol};
    visited[startRow][startCol] = true;
    searching = true;
}

static void buildPath(void) {
    Cell current = {goalRow, goalCol};

    while (current.row != -1 && pathLength < MAX_CELLS) {
        path[pathLength++] = current;

        if (current.row == startRow &&
            current.col == startCol) {
            break;
        }

        current = parent[toIndex(current.row, current.col)];
    }

    // Reverse the path so it runs from start to goal.
    for (int i = 0; i < pathLength / 2; i++) {
        Cell temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }

    pathIndex = 0;
}

static void exploreOneStep(void) {
    if (front >= rear) {
        searching = false;
        finished = true;
        found = false;
        return;
    }

    Cell current = queue[front++];

    if (current.row == goalRow && current.col == goalCol) {
        searching = false;
        finished = true;
        found = true;
        buildPath();
        return;
    }

    for (int d = 0; d < 4; d++) {
        int nr = current.row + dr[d];
        int nc = current.col + dc[d];

        if (nr < 0 || nr >= ROWS ||
            nc < 0 || nc >= COLS) {
            continue;
        }

        if (maze[nr][nc] == 1 || visited[nr][nc]) {
            continue;
        }

        visited[nr][nc] = true;
        parent[toIndex(nr, nc)] = current;

        if (rear < MAX_CELLS) {
            queue[rear++] = (Cell){nr, nc};
        }
    }
}

static bool isPathCell(int row, int col) {
    for (int i = 0; i < pathIndex; i++) {
        if (path[i].row == row && path[i].col == col) {
            return true;
        }
    }
    return false;
}

static void drawMaze(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int x = c * CELL_SIZE;
            int y = TOP_BAR + r * CELL_SIZE;

            Color color = RAYWHITE;

            if (maze[r][c] == 1) {
                color = DARKGRAY;
            } else if (isPathCell(r, c)) {
                color = GREEN;
            } else if (visited[r][c]) {
                color = SKYBLUE;
            }

            if (r == startRow && c == startCol) {
                color = GOLD;
            }

            if (r == goalRow && c == goalCol) {
                color = RED;
            }

            DrawRectangle(x, y, CELL_SIZE - 1, CELL_SIZE - 1, color);
        }
    }
}

int main(void) {
    const int screenWidth = COLS * CELL_SIZE;
    const int screenHeight = TOP_BAR + ROWS * CELL_SIZE;

    InitWindow(screenWidth, screenHeight, "BFS Maze Pathfinder");
    SetTargetFPS(60);

    createMaze();
    resetSearch();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE) && !searching) {
            startBFS();
        }

        if (IsKeyPressed(KEY_R)) {
            resetSearch();
        }

        if (searching) {
            timer += dt;

            // Lower this value to make BFS faster.
            if (timer >= 0.06f) {
                exploreOneStep();
                timer = 0.0f;
            }
        }

        if (finished && found && pathIndex < pathLength) {
            timer += dt;

            if (timer >= 0.025f) {
                pathIndex++;
                timer = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("BFS MAZE PATHFINDER", 10, 8, 22, DARKBLUE);
        DrawText("SPACE: Solve   R: Reset", 10, 39, 16, DARKGRAY);

        if (searching) {
            DrawText("Searching...", 365, 10, 16, BLUE);
        } else if (finished && found) {
            DrawText("PATH FOUND!", 365, 10, 16, DARKGREEN);
        } else if (finished) {
            DrawText("NO PATH", 365, 10, 16, RED);
        } else {
            DrawText("Ready", 365, 10, 16, DARKGRAY);
        }

        drawMaze();

        DrawText("START", 4, TOP_BAR + CELL_SIZE + 4, 10, BLACK);
        DrawText("EXIT", (COLS - 2) * CELL_SIZE,
                 TOP_BAR + (ROWS - 2) * CELL_SIZE, 10, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
