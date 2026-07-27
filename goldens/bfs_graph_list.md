# BFS (Adjacency List)

Shortest path (by number of edges) on an unweighted graph. **Mark visited at enqueue time** — marking at dequeue lets duplicates flood the queue, ruining O(V+E).

## Intuition

- BFS explores in layers: source (layer 0), its neighbors (layer 1), their neighbors (layer 2), etc. A FIFO queue naturally produces this order.
- First time you reach a node = fewest edges from the source. The `prev` array records the parent that discovered it, so the path can be reconstructed backwards.
- Mark `seen` the moment you push a node, not when you pop it. Otherwise the same node can be enqueued multiple times by different neighbors before any of them is popped.
- O(V+E): each vertex enters the queue once, each edge is examined once.
- Weights in the adjacency list are ignored — BFS measures hops, not sum.

## BFS loop

```cpp
using namespace std;

struct Edge { int to; int weight; };

optional<vector<int>> bfs(const vector<vector<Edge>>& g, int source, int sink) {
    int n = (int)g.size();
    vector<bool> seen(n, false);
    vector<int> prev(n, -1);
    queue<int> q;
    seen[source] = true;
    q.push(source);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == sink) break;
        for (auto& e : g[u]) {
            if (seen[e.to]) continue;
            seen[e.to] = true;
            prev[e.to] = u;
            q.push(e.to);
        }
    }

    if (prev[sink] == -1 && source != sink) return nullopt;

    vector<int> path;
    for (int at = sink; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- `seen[neighbor] = true` **inside the for loop** before pushing to the queue — always.
- The early break on `u == sink` is safe: the first time the dequeue reaches the sink, it found it at minimum depth.
- Path reconstruction walks `prev` backwards from sink to source. Returns empty vector if no path exists.

## Where it shows up

- Shortest path in social networks ("degrees of separation"), web crawling, peer-to-peer file sharing.
- Connected components / bipartite checking — run BFS from every unvisited node.
- "Word ladder" shortest transformation sequence, "minimum knight moves" on a board.

## Cousins & contrasts

- **DFS**: stack-based, finds *a* path but not the shortest; can go arbitrarily deep before exploring siblings. O(V+E) as well.
- **Dijkstra**: BFS for non-negative weights — priority queue keyed by cumulative distance. Degrades to BFS when all weights are 1.
- **0-1 BFS**: deque for graphs with only 0/1 weights; push front for 0-weight edges, back for 1-weight. O(V+E).
