# Dijkstra's Shortest Path

Single-source shortest paths on a weighted graph with **non-negative** edge weights. Greedy: the unsettled node with the smallest tentative distance is already final — lock it in.

## Intuition

- Maintain `dist[v]` = best known distance from source; start with 0 at source, ∞ elsewhere.
- Each round, settle the unsettled node `u` with the smallest `dist[u]`. Because no edge is negative, no future detour can improve it — the claim is a proof by "any alternate route exits the settled set through a non-negative edge, so it can only be longer".
- Relax every edge `u → v`: `dist[v] = min(dist[v], dist[u] + w)`. Store `prev[v] = u` whenever you improve, so the path can be rebuilt backwards at the end.
- Negative edges break the greedy argument (a settled node could still improve) → use Bellman-Ford there.
- Complexity: O(V²) with a linear scan for the minimum, O(E log V) with a min-heap.

## O(V²) — linear scan (simple, good for dense graphs)

```cpp
using namespace std;

struct Edge { int to; int weight; };

optional<vector<int>> dijkstra(const vector<vector<Edge>>& g, int source, int sink) {
    int n = (int)g.size();
    vector<int>  dist(n, INT_MAX), prev(n, -1);
    vector<bool> seen(n, false);
    dist[source] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;                                  // min unsettled node
        for (int i = 0; i < n; ++i)
            if (!seen[i] && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || u == sink) break;             // done / unreachable
        seen[u] = true;

        for (auto& e : g[u])                         // relax outgoing edges
            if (!seen[e.to] && dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                prev[e.to] = u;
            }
    }

    if (prev[sink] == -1 && source != sink) return nullopt;

    vector<int> path;                                // rebuild sink→source
    for (int at = sink; at != -1; at = prev[at]) path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- Early exit at `u == sink` is safe: sink is settled the moment it becomes the minimum.
- Pitfall: never relax *into* a settled node; the `seen` check enforces it.
- Pitfall: `dist[u] + e.weight` overflows if `dist[u] == INT_MAX` — the `u == -1` break guards it here.

## Alternative: O(E log V) — lazy min-heap (better on sparse graphs)

- Replace the scan with a priority queue of `(dist, node)`; push on every successful relaxation.
- No decrease-key in `std::priority_queue` → allow duplicates, skip stale entries when popped (`d > dist[u]`).
- Wins when E ≪ V²; on dense graphs the plain scan is simpler and has no heap constant factor.

```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
pq.push({0, source});
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d > dist[u]) continue;                       // stale heap entry
    if (u == sink) break;
    for (auto& e : g[u])
        if (d + e.weight < dist[e.to]) {
            dist[e.to] = d + e.weight;
            prev[e.to] = u;
            pq.push({dist[e.to], e.to});
        }
}
```

## Where it shows up

- GPS routing, OSPF/IS-IS network routing, latency- and cost-minimization problems.
- As a subroutine: A* (Dijkstra + heuristic), Johnson's all-pairs algorithm, "cheapest flight with k stops" variants.

## Cousins & contrasts

- **BFS**: Dijkstra with all weights = 1 — a plain queue suffices, O(V + E).
- **Bellman-Ford**: tolerates negative edges and detects negative cycles, but O(V·E).
- **Prim's MST**: nearly identical code — but the key is the single edge weight, not the path sum. Same greedy skeleton, different quantity being minimized.
- **A***: same relaxation loop plus an admissible heuristic `h(u)` that steers search toward the goal.
