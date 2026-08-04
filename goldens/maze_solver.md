# Maze Solver -- push on enter, unmark and pop on retreat: DFS backtracking

## Core idea
- Seen marks "on the current path", not "visited forever": unmark on retreat so other branches can reuse cells.
- The recursion stack and the path vector grow and shrink together, so at the end cell the vector is exactly the route.

## Build up
1. **Reject walls and seen**

```
if (wall.find(maze[y][x]) != string::npos || seen[y][x]) return false;
```

2. **Mark, push, check the end**

```
seen[y][x] = true;
path.push_back(cur);
if (y == end.y && x == end.x) return true;
```

3. **Dive into each direction**

```
for (auto& d : DIRS)
    if (walk(maze, wall, {x + d[1], y + d[0]}, end, seen, path)) return true;
```

4. **Unmark and pop on retreat**

```
path.pop_back();
seen[y][x] = false;
return false;
```

## Diagram
```
start (10,0)                    x = wall, . = open
  | down
(10,4)                          row 4: x . . . . . . . . . . x
  left along row 4 -> (1,4)
(1,4)
  | up: (1,3) (1,2) (1,1)       dead-end pocket -> unmark, pop
  | down
(1,5) end                       final route: down, left, down
```

## Approach -- DFS backtracking
```cpp
using namespace std;

static const int DIRS[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};  // up right down left

static bool walk(const vector<string>& maze, const string& wall, Point cur, Point end,
                 vector<vector<bool>>& seen, vector<Point>& path) {
    int y = cur.y, x = cur.x;
    if (y < 0 || y >= (int)maze.size() || x < 0 || x >= (int)maze[y].size()) return false;
    if (wall.find(maze[y][x]) != string::npos || seen[y][x]) return false;  // step 1

    seen[y][x] = true;
    path.push_back(cur);                                 // step 2: extend the path
    if (y == end.y && x == end.x) return true;           // step 2: done

    for (auto& d : DIRS)                                 // step 3: dive each way
        if (walk(maze, wall, {x + d[1], y + d[0]}, end, seen, path))
            return true;                                 // step 3: success floods up

    path.pop_back();                                     // step 4: unmark, retreat
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

- walk is steps 1-4 in order; the trailing unmark/pop tail is exactly the backtracking step.
- The end-check return floods the result up intact, so solve_maze just seeds seen and path.

### Trace
- From (10,0) only down is open: dive to (10,4), walk row 4 left to (1,4); the up-pocket (1,3)(1,2)(1,1) exhausts and pops; down reaches (1,5). Path: column 10, row 4, then down.

## Complexity
- O(R*C) time, O(R*C) space for seen, the path, and the recursion stack.

## Alternative -- BFS shortest path
- A queue plus a prev array finds the fewest-move route instead of any route.
- Costs a frontier that can dwarf the DFS stack; use it when the kata asks for shortest.

## Use when
- Any path through a grid: reach for this when the question is "is there a way, and what is it" (robot pathfinding, puzzle solvers).
- Deep narrow maps: reach for this when O(depth) recursion beats a ballooning frontier.

## Cousins
- Plain graph DFS: marks visited forever; maze DFS must unmark on retreat.
- BFS: shortest grid path, queue plus prev instead of recursion plus unmark.
- A*: Dijkstra plus a heuristic that steers toward the end cell.
