# DFS on a Graph (Adjacency List)

Depth-first search -- explores as far as possible along each branch before backtracking. Finds **a** path from source to sink (not necessarily the shortest).

## Intuition

- Recursive skeleton: mark -> recurse children -> (optionally unmark). This is the template for all backtracking.
- Unlike BFS, DFS does not need a queue; the call stack acts as the implicit stack.
- O(V+E): each node visited once, each edge examined once.
- For path search, `prev` records the parent and recursion stops early when the sink is found.

## Approach 1 -- recursive DFS

```cpp
using namespace std;

struct Edge { int to; int weight; };

bool walk(const vector<vector<Edge>>& g, int u, int sink,
          vector<bool>& seen, vector<int>& prev) {
    if (u == sink) return true;
    seen[u] = true;
    for (auto& e : g[u]) {
        if (seen[e.to]) continue;
        prev[e.to] = u;
        if (walk(g, e.to, sink, seen, prev))
            return true;
    }
    return false;
}

optional<vector<int>> dfs(const vector<vector<Edge>>& g, int source, int sink) {
    int n = (int)g.size();
    vector<bool> seen(n, false);
    vector<int> prev(n, -1);

    if (!walk(g, source, sink, seen, prev))
        return nullopt;

    vector<int> path;
    for (int at = sink; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- Mark `seen` before recursing, not after -- prevents revisiting a node in the same call chain.
- The walk returns `bool` so it can short-circuit the entire recursion once the sink is found.
- For traversal (visit every node), omit the early return and make walk return void.

## Approach 2 -- iterative DFS (explicit stack)

```cpp
stack<int> st;
st.push(source);
seen[source] = true;
while (!st.empty()) {
    int u = st.top(); st.pop();
    if (u == sink) { /* found */ break; }
    for (auto& e : g[u])
        if (!seen[e.to]) { seen[e.to] = true; prev[e.to] = u; st.push(e.to); }
}
```

- Use a stack instead of recursion to avoid call-stack overflow on deep graphs.
- Push neighbors in reverse adjacency order to match the recursive visitation order.

## Alternative -- DFS with three colors (cycle detection)

- Add an `onStack` array alongside `seen`. A back edge to a node still `onStack` reveals a cycle.
- The same idea generalizes to strongly connected components (Tarjan, Kosaraju) and 2-SAT.
- Track pre/post times for interval arithmetic: edge u->v is a tree/back/forward/cross edge depending on where v's interval sits relative to u's.

## Complexity

- Time: O(V+E).
- Space: O(V) for seen/prev, plus O(V) recursion depth in the worst case.

## Usage

- Cycle detection: a back edge points to an ancestor still on the recursion stack.
- Topological sort via DFS post-order: append node after recursing all children, reverse at the end.
- Connected components, bipartite coloring, maze solving, Sudoku/N-Queens backtracking.
- Strongly connected components (Tarjan, Kosaraju -- both build on DFS).
- Anywhere "go as deep as you can, then back up" matches the problem's natural shape.

## Cousins & contrasts

- **BFS**: finds the shortest path, O(V+E). DFS uses a stack; BFS uses a queue.
- **Backtracking**: DFS with state mutation and unmarking -- exhaustive search over a decision space.
- **Iterative DFS**: avoids recursion depth limits; same logic, different container.
- **Dijkstra**: BFS for non-negative weights -- DFS has no early-stop guarantee for weighted shortest paths.
