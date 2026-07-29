# Maze Solver

Find **any** path from start to end in a grid of open/wall cells using DFS with backtracking.

## Intuition

- DFS plunges down one corridor until it hits the end or a dead end; on a dead end it retreats and tries the next direction. The recursion stack *is* the current path.
- Mark a cell visited **on entry** so you never loop; **unmark on retreat** so other paths may reuse the cell -- that's the backtracking step plain graph-DFS skips.
- Collect the path in a vector: push on enter, pop on exit. When the end cell is reached, the vector holds exactly the solution.
- O(R*C) time, O(R*C) worst-case recursion depth.

## Approach -- DFS backtracking

```cpp
using namespace std;

static const int DIRS[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};  // up right down left

static bool walk(const vector<string>& maze, const string& wall, Point cur, Point end,
                 vector<vector<bool>>& seen, vector<Point>& path) {
    int y = cur.y, x = cur.x;
    if (y < 0 || y >= (int)maze.size() || x < 0 || x >= (int)maze[y].size()) return false;
    if (wall.find(maze[y][x]) != string::npos || seen[y][x]) return false;

    seen[y][x] = true;
    path.push_back(cur);
    if (y == end.y && x == end.x) return true;

    for (auto& d : DIRS)
        if (walk(maze, wall, {x + d[1], y + d[0]}, end, seen, path))
            return true;

    path.pop_back();
    seen[y][x] = false;
    return false;
}

vector<Point> solve_maze(const vector<string>& maze, const string& wall, Point start, Point end) {
    vector<vector<bool>> seen(maze.size(), vector<bool>(maze[0].size(), false));
    vector<Point> path;
    walk(maze, wall, start, end, seen, path);
    return path;
}
```

- Pitfall: forgetting `seen[y][x] = false` on retreat -> cells burned by failed attempts block valid detours.
- Success short-circuits with `return true` all the way up, preserving the path exactly.
- Bounds/wall/seen checks at function entry keep the loop body to three lines.

## Alternative -- BFS (shortest path)

- Replace DFS with BFS and keep a `prev` array for path reconstruction. O(R*C) time, gives the **shortest** path in unweighted grids.
- Costs O(R*C) extra memory for the queue and `prev` array; pays off when the kata asks for shortest or minimum steps.

## Alternative -- A* (faster shortest path on big maps)

- BFS + admissible heuristic (e.g. Manhattan distance) to steer the search toward the goal. Heuristic-heavy, but explores far fewer cells on large grids.
- Drop the heuristic and you get Dijkstra over a 4-connected grid; drop the priority queue and you get plain BFS.

## Alternative -- bidirectional BFS (huge state spaces)

- BFS from start and from end simultaneously; stop when the two frontiers meet. Roughly halves the search diameter.
- Best when the grid is enormous and the heuristic-free uniform-cost approach is too slow.

## Complexity

- Time: O(R*C) -- every cell is visited at most a small constant number of times.
- Space: O(R*C) for `seen`, path, and recursion stack.

## Usage

- Robot pathfinding, puzzle solvers (sudoku-style backtracking shares the skeleton), PCB routing.
- "Find any feasible solution" where depth-first memory beats BFS's frontier.
- Game AI pathfinding (though A* is more common in practice for shortest paths).
- Any "is there a way through this grid, and if so, what is it" question.

## Cousins & contrasts

- **BFS**: gives the **shortest** path in an unweighted grid; this DFS gives *any* path but dives deep first.
- **Plain graph DFS**: marks visited permanently; maze backtracking must unmark because "visited on this path" != "visited globally".
- **A***: Dijkstra + heuristic when you need the shortest path fast on large maps.
- **Union-find / flood fill**: if you only need to know whether start and end are connected, BFS/DFS without path reconstruction is the simpler tool.
