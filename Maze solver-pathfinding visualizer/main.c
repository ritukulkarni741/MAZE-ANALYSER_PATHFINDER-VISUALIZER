#include "main.h"
#include "pathfinding.h"

#include <stdio.h>
#include <string.h>

bool loadMaze(const char *filename, Maze *maze)
{
    if (filename == NULL || maze == NULL) {
        return false;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    int rows;
    int cols;

    if (fscanf(file, "%d %d", &rows, &cols) != 2) {
        fclose(file);
        return false;
    }

    if (rows < 1 || rows > MAX_ROWS || cols < 1 || cols > MAX_COLS) {
        fclose(file);
        return false;
    }
int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF) {
        if (ch != ' ' && ch != '\t' && ch != '\r') {
            fclose(file);
            return false;
        }
    }

    maze->rows = rows;
    maze->cols = cols;

    int startCount = 0;
    int endCount = 0;
    char line[MAX_COLS + 3];

    for (int row = 0; row < rows; row++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fclose(file);
            return false;
        }

        size_t length = strlen(line);
        while (length > 0 &&
               (line[length - 1] == '\n' || line[length - 1] == '\r')) {
            line[--length] = '\0';
        }

        if (length != (size_t)cols) {
            fclose(file);
            return false;
        }

        for (int col = 0; col < cols; col++) {
            char cell = line[col];

            if (cell != '#' && cell != '.' && cell != 'S' && cell != 'E') {
                fclose(file);
                return false;
            }

            maze->cells[row][col] = cell;

            if (cell == 'S') {
                maze->start.row = row;
                maze->start.col = col;
                startCount++;
            } else if (cell == 'E') {
                maze->end.row = row;
                maze->end.col = col;
                endCount++;
            }
        }
    }



while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t') {
            fclose(file);
            return false;
        }
    }

    fclose(file);
    return startCount == 1 && endCount == 1;
}

void printMaze(const Maze *maze, bool path[MAX_ROWS][MAX_COLS], const char *title)
{
    if (maze == NULL) {
        return;
    }

    if (title != NULL) {
        printf("%s\n", title);
    }

    for (int row = 0; row < maze->rows; row++) {
        for (int col = 0; col < maze->cols; col++) {
            char cell = maze->cells[row][col];

            if (cell == 'S' || cell == 'E') {
                putchar(cell);
            } else if (path != NULL && path[row][col]) {
                putchar('*');
            } else {
                putchar(cell);
            }
        }
        putchar('\n');
    }
}

int main(void)
{
    Maze maze;

    if (!loadMaze("maze.txt", &maze)) {
        fprintf(stderr, "Could not load maze.txt. Check the file and its format.\n");
        return 1;
    }

    printf("Loaded maze: %d rows x %d columns\n\n", maze.rows, maze.cols);
    printMaze(&maze, NULL, "Original maze:");

    bool dfsPath[MAX_ROWS][MAX_COLS] = {{false}};
    int dfsVisited = 0;

    printf("\n--- Depth-First Search (DFS) ---\n");
    if (solveDFS(&maze, dfsPath, &dfsVisited)) {
        printMaze(&maze, dfsPath, "Solution (* marks the path):");
    } else {
        printf("No path exists from S to E.\n");
    }
    printf("Cells visited by DFS: %d\n", dfsVisited);

    bool bfsPath[MAX_ROWS][MAX_COLS] = {{false}};
    int bfsVisited = 0;

    printf("\n--- Breadth-First Search (BFS) ---\n");
    if (solveBFS(&maze, bfsPath, &bfsVisited)) {
        printMaze(&maze, bfsPath, "Shortest solution (* marks the path):");
    } else {
        printf("No path exists from S to E.\n");
    }
    printf("Cells discovered by BFS: %d\n", bfsVisited);

    return 0;
}
