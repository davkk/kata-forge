# Prim's MST -- grow one tree by always adding the cheapest edge that connects a new node

## Core idea
- Cut property: the cheapest edge leaving the growing tree crosses the cut "tree | rest" at minimum weight, so it belongs to some MST.
- key[v] holds that cheapest edge weight from the tree to v, parent[v] its tree endpoint; settling the smallest key locks it as final.

## Build up
1. **Pick the nearest to tree**

```
int u = -1;
for (int i = 0; i < n; i++)
    if (!inMST[i] && (u == -1 || key[i] < key[u])) u = i;
```

2. **Join it to the tree**

```
inMST[u] = true;
```

3. **Cheapest single edge wins**

```
if (e.weight < key[e.to]) {
    key[e.to] = e.weight;
    parent[e.to] = u;
}
```

4. **Build MST from parents**

```
mst[parent[v]].push_back({v, key[v]});
mst[v].push_back({parent[v], key[v]});
```

## Diagram
```
cut = tree | rest;  key[v] = cheapest edge weight from tree to v
  settle order       winning cut edge
  0 (root)           none
  2 (key 1)          0-2
  1 (key 3)          0-1
  4 (key 1)          1-4
  5 (key 2)          4-5
  6 (key 1)          5-6
  3 (key 1)          6-3
MST: 0-2 0-1 1-4 4-5 5-6 6-3, total 9
```

## Approach -- linear-scan grow
```cpp
using namespace std;

optional<vector<vector<Edge>>> prims(const vector<vector<Edge>>& g) {
    int n = (int)g.size();
    vector<bool> inMST(n, false);
    vector<int>  key(n, INT_MAX);
    vector<int>  parent(n, -1);
    key[0] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;
        for (int i = 0; i < n; ++i)                    // step 1: nearest to tree
            if (!inMST[i] && (u == -1 || key[i] < key[u])) u = i;
        if (u == -1) break;                            // disconnected
        inMST[u] = true;                               // step 2: join the tree

        for (auto& e : g[u])                           // step 3: relax one edge
            if (!inMST[e.to] && e.weight < key[e.to]) {
                key[e.to] = e.weight;
                parent[e.to] = u;
            }
    }

    vector<vector<Edge>> mst(n);                       // step 4: from parents
    for (int v = 1; v < n; ++v) {
        if (parent[v] == -1) return nullopt;           // disconnected
        int u = parent[v];
        mst[u].push_back({v, key[v]});
        mst[v].push_back({u, key[v]});
    }
    return mst;
}
```

- Steps 1-3 mirror Dijkstra exactly, except the key is a single edge weight, not a path sum.
- Step 4 reads parent/key straight into the adjacency list; any missing parent means disconnected.

### Trace
- Settle order 0, 2, 1, 4, 5, 6, 3; keys settle at 2=1 (0-2), 1=3 (0-1), 4=1 (1-4), 5=2 (4-5), 6=1 (5-6), 3=1 (6-3). MST total 9.

## Complexity
- O(V^2) time with the scan (O(E log V) with a heap), O(V) space.

## Alternative -- Kruskal
- Sort all edges and union-find rejects cycles (see kruskals).
- Better when edges arrive as a flat list instead of an adjacency structure.

## Use when
- Dense node-first graphs: reach for this when you need a minimum spanning tree from an adjacency list (grids, road networks).
- Approximation bounds: reach for this when an MST lower bound matters (Steiner tree, metric TSP).

## Cousins
- Dijkstra: identical skeleton; the key is edge weight vs path sum, and Prim cannot stop early.
- Reverse-delete: sort descending and drop edges that keep the graph connected.
- Boruvka: adds every component's cheapest edge at once; parallel-friendly.
