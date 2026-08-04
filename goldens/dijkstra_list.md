# Dijkstra's Shortest Path -- the nearest unsettled node settles first, and its cost is final

## Core idea
- Invariant: the unsettled node with the smallest dist cannot be improved, since any detour to it must exit the settled set over a non-negative edge.
- dist/prev hold the best known cost and its parent; settling in ascending cost order is the greedy that matches the optimum.

## Build up
1. **Scan for the nearest**

```
int u = -1;
for (int i = 0; i < n; i++)
    if (!seen[i] && (u == -1 || dist[i] < dist[u])) u = i;
```

2. **Settle: cost is final**

```
seen[u] = true;
```

3. **Relax: try a cheaper way**

```
if (dist[u] + e.weight < dist[e.to]) {
    dist[e.to] = dist[u] + e.weight;
    prev[e.to] = u;
}
```

4. **Stop at the sink**

```
if (u == -1 || u == sink) break;
```

## Diagram
```
dist labels; [x] = settled (final):
  node    0    1    2    3    4    5    6
  dist    0    3    1    8    4    6    7
         [0]  [3]  [1]   (8)  [4]  [6]  [7]
  settle order: 0 -> 2 -> 1 -> 4 -> 5 -> 6, stop at sink
  relax only lowers dist: 2->1 = 5 worse than 3, keep 3
  prev: 6 <- 5 <- 4 <- 1 <- 0  =>  [0 1 4 5 6], sum 7
```

## Approach -- linear-scan settle
```cpp
using namespace std;

optional<vector<int>> dijkstra(const vector<vector<Edge>>& g, int source, int sink) {
    int n = (int)g.size();
    vector<int>  dist(n, INT_MAX), prev(n, -1);
    vector<bool> seen(n, false);
    dist[source] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;                                  // step 1: scan for nearest
        for (int i = 0; i < n; ++i)
            if (!seen[i] && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || u == sink) break;             // step 4: done / sink settled
        seen[u] = true;                              // step 2: settle, cost final

        for (auto& e : g[u])                         // step 3: relax neighbors
            if (!seen[e.to] && dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                prev[e.to] = u;
            }
    }

    if (prev[sink] == -1 && source != sink) return nullopt;

    vector<int> path;                                // walk prev backwards
    for (int at = sink; at != -1; at = prev[at]) path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}
```

- The scan is step 1; settling (step 2) locks the cost, and relax (step 3) is the only place dist/prev change.
- The `!seen[e.to]` guard never relaxes into a settled node, which is the non-negative-weights assumption, line by line.

### Trace
- Settle order 0 (0), 2 (1), 1 (3), 4 (4), 5 (6), 6 (7) then stop; relax 2->1 = 5 rejected, 1->4 = 4 accepted. Path [0,1,4,5,6], sum 7.
- Sink 3 is unreachable from 0, so dijkstra returns nullopt.

## Complexity
- O(V^2) time with the scan (O(E log V) with a heap), O(V) space.

## Alternative -- lazy min-heap
- A priority_queue of (dist, node) replaces the scan; on pop, `if (d > dist[u]) continue;` skips stale entries.
- O(E log V), best when E is far below V^2.

## Use when
- Weighted shortest path: reach for this when edges carry non-negative costs and you want the minimum sum (GPS routing, cheapest flights, OSPF).
- Primitive routine: reach for this as the core of A* and Johnson's algorithm.

## Cousins
- BFS: Dijkstra with every edge weight 1; a plain queue.
- Bellman-Ford: tolerates negative edges, O(V*E); Dijkstra breaks on them.
- Prim: the same settle skeleton, but the key is a single edge weight, not a path sum.
