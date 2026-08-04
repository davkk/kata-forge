# DFS on a Graph -- plunge to the dead end, back up to the last fork, try the next branch

## Core idea
- Recursion is the trail: commit to one branch, and the call stack holds every fork you can back up to.
- Mark seen on entry stops cycles; first-found path is a path, not necessarily the shortest.

## Build up
1. **Sink ends the dive**

```
if (u == sink) return true;
```

2. **Mark seen on entry**

```
seen[u] = true;
```

3. **Record parent, plunge**

```
prev[e.to] = u;
if (walk(g, e.to, sink, seen, prev)) return true;
```

4. **Dead end backs out**

```
return false;
```

## Diagram
```
walk(0)
  walk(1)                prev[4] = 1
    walk(4)              prev[3] = 4, prev[5] = 4
      walk(3)            no open neighbor -> false, backs out
      walk(5)            prev[6] = 5
        walk(6)          == sink -> true floods upward
path: [0 1 4 5 6]        the recursion stack is the trail
```

## Approach -- recursive walk
```cpp
using namespace std;

bool walk(const vector<vector<Edge>>& g, int u, int sink,
          vector<bool>& seen, vector<int>& prev) {
    if (u == sink) return true;           // step 1: sink ends the dive
    seen[u] = true;                       // step 2: mark on entry
    for (auto& e : g[u]) {
        if (seen[e.to]) continue;
        prev[e.to] = u;                   // step 3: record parent, plunge
        if (walk(g, e.to, sink, seen, prev))
            return true;                  // step 3: branch succeeded
    }
    return false;                         // step 4: dead end backs out
}

optional<vector<int>> dfs(const vector<vector<Edge>>& g, int source, int sink) {
    int n = (int)g.size();
    vector<bool> seen(n, false);
    vector<int> prev(n, -1);

    if (!walk(g, source, sink, seen, prev))
        return nullopt;

    vector<int> path;                     // rebuild: walk prev backwards
    for (int at = sink; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- walk is steps 1-4 stacked; the `if (walk(...)) return true;` unwinds the whole stack on success, so prev keeps only winning parents.
- The rebuild loop is the same walk-back BFS uses; a failed walk returns nullopt.

### Trace
- walk(0) -> 1 -> 4: child 3 dead-ends and pops, child 5 -> 6 == sink floods true -> [0,1,4,5,6].
- From 6 the recursion never reaches 0, so dfs returns nullopt.

## Complexity
- O(V+E) time, O(V) space: each node visited once, plus recursion depth.

## Alternative -- iterative stack
- A `stack<int>` plus a while loop replaces the call stack; push neighbors in reverse order to match recursive order.
- Use it on very deep graphs near recursion limits.

## Use when
- Existence and exploration: reach for this when the question is "does a path exist" or "visit everything" (reachability, connected components).
- Backtracking: reach for this when you mutate state, explore, then undo (Sudoku, N-Queens).

## Cousins
- BFS: breadth-first sibling; shortest path in unweighted graphs.
- Backtracking: DFS plus unmarking; exhaustive search over a decision space.
- Cycle detection: a 3-color DFS (onStack flag) is the standard test.
