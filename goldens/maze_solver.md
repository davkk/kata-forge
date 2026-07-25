# Maze Solver

Find **any** path from start to end in a grid of open/wall cells using DFS with backtracking.

## Intuition

- DFS plunges down one corridor until it hits the end or a dead end; on a dead end it retreats and tries the next direction. The recursion stack *is* the current path.
- Mark a cell visited **on entry** so you never loop; **unmark on retreat** so other paths may reuse the cell — that's the backtracking step plain graph-DFS skips.
- Collect the path in a vector: push on enter, pop on exit. When the end cell is reached, the vector holds exactly the solution.
- Complexity: O(R·C) time, O(R·C) worst-case recursion depth.

## DFS backtracking

```cpp
using namespace std;

struct Point { int x, y; };

static const int DIRS[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};  // up right down left

static bool walk(const vector<string>& maze, char wall, Point cur, Point end,
                 vector<vector<bool>>& seen, vector<Point>& path) {
    int y = cur.y, x = cur.x;
    if (y < 0 || y >= (int)maze.size() || x < 0 || x >= (int)maze[y].size()) return false;
    if (maze[y][x] == wall || seen[y][x]) return false;

    seen[y][x] = true;
    path.push_back(cur);
    if (y == end.y && x == end.x) return true;

    for (auto& d : DIRS)
        if (walk(maze, wall, {y + d[0], x + d[1]}, end, seen, path))
            return true;

    path.pop_back();
    seen[y][x] = false;
    return false;
}

vector<Point> solve(const vector<string>& maze, char wall, Point start, Point end) {
    vector<vector<bool>> seen(maze.size(), vector<bool>(maze[0].size(), false));
    vector<Point> path;
    walk(maze, wall, start, end, seen, path);
    return path;
}
```

- Pitfall: forgetting `seen[y][x] = false` on retreat → cells burned by failed attempts block valid detours.
- Success short-circuits with `return true` all the way up, preserving the path exactly.
- Bounds/wall/seen checks at function entry keep the loop body to three lines.

## Where it shows up

- Robot pathfinding, puzzle solvers (sudoku-style backtracking shares the skeleton), PCB routing.
- "Find any feasible solution" where depth-first memory beats BFS's frontier.

## Cousins & contrasts

- **BFS**: gives the **shortest** path in an unweighted grid; this DFS gives *any* path but dives deep first.
- **Plain graph DFS**: marks visited permanently; maze backtracking must unmark because "visited on this path" ≠ "visited globally".
- **A***: Dijkstra + heuristic when you need the shortest path fast on large maps.
