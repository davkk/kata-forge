# BFS (Adjacency Matrix)

Shortest path (by edges) on a graph stored as a dense adjacency matrix where `matrix[i][j] != 0` means an edge from i to j.

## Intuition

- Same BFS logic as the adjacency-list version, but finding neighbors requires scanning the entire row — O(V) per node instead of O(deg(u)).
- For each dequeue `u`, iterate `v = 0 .. n-1` and check `matrix[u][v] != 0`.
- O(V²) time regardless of actual edge count. Prefer an adjacency list when the graph is sparse.
- The path reconstruction (`prev` array, walk backwards from sink) is identical to the list version.

## BFS on a matrix

```cpp
using namespace std;

vector<int> bfs(const vector<vector<int>>& matrix, int source, int sink) {
    int n = (int)matrix.size();
    vector<bool> seen(n, false);
    vector<int> prev(n, -1);
    queue<int> q;
    seen[source] = true;
    q.push(source);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == sink) break;
        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] == 0) continue;
            if (seen[v]) continue;
            seen[v] = true;
            prev[v] = u;
            q.push(v);
        }
    }

    if (prev[sink] == -1 && source != sink) return {};

    vector<int> path;
    for (int at = sink; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- The only difference from the list version is the inner loop: `for (int v = 0; v < n; ++v)` scanning every column.
- Edge predicate `matrix[u][v] != 0` works for any positive weight. Change to `== 1` for purely unweighted.
- Keep the seen-early mark: `seen[v] = true` before pushing, not after popping.

## Where it shows up

- Dense graphs where E ≈ V² and the adjacency list's overhead is not worth it.
- Grid-based mazes: treat the grid as an implicit graph where up/down/left/right are the only edges, and bounds checks replace the adjacency structure. The BFS algorithm is identical — the grid IS the graph.

## Cousins & contrasts

- **BFS on adjacency list**: O(V+E) instead of O(V²); always preferred for sparse graphs.
- **DFS on matrix**: same neighbor-scan loop but with a stack — no shortest-path guarantee.
- **Grid BFS**: same queue + seen pattern but neighbors are computed from (r,c) instead of read from matrix[u][v]. Use encoded position `r*cols+c` to pack both coordinates into one int.
