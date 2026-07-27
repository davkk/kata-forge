# BFS on a Graph (Adjacency List)

Shortest path (by number of edges) on an unweighted graph. **Mark visited at enqueue time** -- marking at dequeue lets duplicates flood the queue, ruining O(V+E).

## Intuition

- BFS explores in layers: source (layer 0), its neighbors (layer 1), their neighbors (layer 2), etc. A FIFO queue naturally produces this order.
- First time you reach a node = fewest edges from the source. The `prev` array records the parent that discovered it, so the path can be reconstructed backwards.
- Mark `seen` the moment you push a node, not when you pop it. Otherwise the same node can be enqueued multiple times by different neighbors before any of them is popped.
- O(V+E): each vertex enters the queue once, each edge is examined once.
- Weights in the adjacency list are ignored -- BFS measures hops, not sum.

## Approach -- BFS loop with prev reconstruction

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

- `seen[neighbor] = true` **inside the for loop** before pushing to the queue -- always.
- The early break on `u == sink` is safe: the first time the dequeue reaches the sink, it found it at minimum depth.
- Path reconstruction walks `prev` backwards from sink to source. Returns empty vector if no path exists.

## Alternative -- 0-1 BFS (edge weights 0 or 1)

- Replace the `queue` with a `deque`. Push to the front for 0-weight edges, push to the back for 1-weight edges.
- Same O(V+E) but handles edge weights in {0, 1} without a heap. Common in shortest-path-on-a-grid katas where moves cost 0 or 1.

## Alternative -- bidirectional BFS (huge state spaces)

- Run BFS from both source and sink, alternating one layer at a time. Stop when the two frontiers meet.
- Cuts the explored space from `b^d` to roughly `2 * b^(d/2)` -- exponential win when the branching factor is large.

## Complexity

- Time: O(V+E).
- Space: O(V) for the queue, seen, and prev arrays.

## Usage

- Shortest path in social networks ("degrees of separation"), web crawling, peer-to-peer file sharing.
- Connected components / bipartite checking -- run BFS from every unvisited node.
- Minimum number of moves on a board (knight, rook, king) when each move has the same cost.
- Garbage-collecting reachability: "what is reachable from this object/address/URL" without caring about the order.

## Cousins & contrasts

- **DFS**: stack-based, finds *a* path but not the shortest; can go arbitrarily deep before exploring siblings. O(V+E) as well.
- **Dijkstra**: BFS for non-negative weights -- priority queue keyed by cumulative distance. Degrades to BFS when all weights are 1.
- **A***: BFS + admissible heuristic that steers search toward the goal -- same relaxation loop, lower constant.
- **BFS on a tree**: same loop without the `seen` array -- trees have no cycles so every visit is fresh.
