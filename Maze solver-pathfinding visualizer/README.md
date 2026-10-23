# Maze Solver and Pathfinding Visualizer

## About the Project

This is a text-based maze solver written in C. It loads a maze from `maze.txt`, checks that the input is valid, and searches for a route from the start (`S`) to the exit (`E`).

The project implements two algorithms:

- **Depth-First Search (DFS):** Explores a path deeply and backtracks when it reaches a dead end.
- **Breadth-First Search (BFS):** Explores nearby cells first and finds a shortest route when every move has the same cost.

The program prints the original maze and the solutions found by the algorithms. An asterisk (`*`) marks the selected route. It also reports how many cells each algorithm marked as visited or discovered.

## Maze Symbols

| Symbol | Meaning |
|---|---|
| `#` | Wall; cannot be entered |
| `.` | Open cell; can be entered |
| `S` | Starting position; include exactly one |
| `E` | Exit or destination; include exactly one |
| `*` | Route marker in the program output |

## Input File Format

The first line contains the number of rows and columns, separated by a space. Each following line represents one row and must contain exactly the specified number of characters.

Example:

```text
7 11
###########
#S..#.....#
#.#.#.###.#
#.#...#...#
#.#####.#E#
#.......#.#
###########
```

The program supports mazes from 1 to 50 rows and 1 to 50 columns.

Every maze must contain exactly one `S` and exactly one `E`.

## Files in This Project

- `main.c` — Program entry point, input loading, validation, and output.
- `main.h` — Shared constants, structures, and maze function declarations.
- `pathfinding.c` — DFS and BFS implementations.
- `pathfinding.h` — Pathfinding function declarations.
- `queue.c` — Circular-array queue implementation used by BFS.
- `queue.h` — Queue structure and function declarations.
- `maze.txt` — Sample maze input.
- `README.md` — Project documentation and instructions.

## How to Compile

Open the VS Code terminal in the folder containing these files.

### Windows (GCC / MinGW)

```powershell
gcc -std=c11 -Wall -Wextra -pedantic main.c pathfinding.c queue.c -o maze_solver.exe
```

### Linux or macOS

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c pathfinding.c queue.c -o maze_solver
```

## How to Run

Run the program from the project folder so it can find `maze.txt`.

### Windows PowerShell

```powershell
.\maze_solver.exe
```

### Windows Command Prompt

```bat
maze_solver.exe
```

### Linux or macOS

```bash
./maze_solver
```

You can also pass a different maze filename to the program.

For Windows:

```powershell
.\maze_solver.exe another_maze.txt
```

For Linux or macOS:

```bash
./maze_solver another_maze.txt
```

## How the Algorithms Work

### Depth-First Search (DFS)

DFS uses recursion to explore the maze. It marks each cell it visits, tries the four neighbouring directions, and returns to an earlier decision point when a route leads to a dead end.

### Breadth-First Search (BFS)

BFS uses a first-in, first-out queue. It explores cells in increasing distance from the starting point. It records the parent of each discovered cell so it can reconstruct and display the shortest route.

## Algorithm Complexity

For a maze with `R` rows and `C` columns, both searches have a worst-case time complexity of:

**O(R × C)**

Each cell has at most four neighbours, and each cell is explored at most once.

The search data can require **O(R × C)** additional memory.

## Limitations

- Movement is allowed only up, down, left, and right.
- Diagonal movement is not supported.
- Every move has the same cost, so BFS finds a shortest route by number of moves.
- This is the text-based version; a graphical visualization can be added later.