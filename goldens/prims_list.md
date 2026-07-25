# Prim's MST

Builds a minimum spanning tree for a connected, undirected, weighted graph. Greedy: repeatedly add the cheapest edge connecting the visited set to an unvisited node.

## Intuition

- **Cut property**: for any partition of vertices into two sets, the minimum-weight edge crossing that cut belongs to every MST. Prim exploits this by growing a single set from one start vertex.
- Maintain `key[v]` = cheapest edge weight from the growing tree to `v`; `parent[v]` = the tree endpoint of that edge.
- Each round, settle the unvisited node with the smallest key. The key is final — any later edge to that node is heavier by construction.
- O(V²) with a linear scan for the minimum; O(E log V) with a min-heap.

## O(V²) — linear scan (mirrors Dijkstra)

```cpp
using namespace std;

struct Edge { int to, weight; };

vector<vector<Edge>> prims(const vector<vector<Edge>>& g) {
    int n = (int)g.size();
    vector<bool> inMST(n, false);
    vector<int>  key(n, INT_MAX);
    vector<int>  parent(n, -1);
    key[0] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;
        for (int i = 0; i < n; ++i)
            if (!inMST[i] && (u == -1 || key[i] < key[u])) u = i;
        if (u == -1) break;                     // disconnected
        inMST[u] = true;

        for (auto& e : g[u])
            if (!inMST[e.to] && e.weight < key[e.to]) {
                key[e.to] = e.weight;
                parent[e.to] = u;
            }
    }

    vector<vector<Edge>> mst(n);
    for (int v = 1; v < n; ++v) {
        if (parent[v] == -1) return {};         // disconnected
        int u = parent[v];
        mst[u].push_back({v, key[v]});
        mst[v].push_back({u, key[v]});
    }
    return mst;
}
```

- Structurally identical to Dijkstra, but the key is the **single edge weight**, not the path sum. Relaxation: `e.weight < key[e.to]` vs `dist[u] + e.weight < dist[e.to]`.
- Root at node 0; any start vertex works.
- Returns empty graph (null) if the input is disconnected.

## Alternative: O(E log V) — min-heap

- Replace the linear scan with a priority queue. Push `(weight, v)` on every improved key.
- Better on sparse graphs; the constant factor makes it slower than scan on very dense graphs.

```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
pq.push({0, 0});
key[0] = 0;
while (!pq.empty()) {
    auto [w, u] = pq.top(); pq.pop();
    if (inMST[u]) continue;        // stale entry
    inMST[u] = true;
    for (auto& e : g[u])
        if (!inMST[e.to] && e.weight < key[e.to]) {
            key[e.to] = e.weight;
            parent[e.to] = u;
            pq.push({e.weight, e.to});
        }
}
```

## Where it shows up

- Network design: electrical grids, fiber-optic cable, road networks connecting cities.
- Approximation algorithms: MST is a lower bound for Steiner tree and metric TSP.

## Cousins & contrasts

- **Dijkstra**: same skeleton, different key — path sum vs edge weight. Dijkstra can stop early at the sink; Prim always processes all V nodes.
- **Kruskal**: sorts all edges globally and uses Union-Find. Better when the edge list is available directly; Prim is better for dense adjacency-list graphs.
- **Visited set**: Dijkstra relaxes edges from the settled set to unsettled; Prim does the same but picks the cheapest single edge rather than the cheapest path.
