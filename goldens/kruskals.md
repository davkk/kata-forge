# Kruskal's MST

Finds a minimum spanning tree by sorting all edges by weight and adding them one by one if they connect different components -- cycle test via Union-Find.

## Intuition

- Sort all edges by weight -- O(E log E) dominates the runtime.
- Walk the sorted list; for each edge, check whether its endpoints are already in the same component. If not, add it (Union-Find tracks this in near-O(1)).
- The cut property justifies Kruskal: the minimum edge crossing any cut belongs to some MST, and processing in sorted order guarantees we find it.
- Returns nullopt if the graph is disconnected (fewer than n-1 edges added).

## Approach -- Kruskal + Union-Find

```cpp
using namespace std;

struct Edge { int to; int weight; };

struct UF {
    vector<int> p;
    UF(int n) : p(n, -1) {}
    int find(int x) { return p[x] < 0 ? x : (p[x] = find(p[x])); }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (p[a] > p[b]) swap(a, b);
        p[a] += p[b]; p[b] = a;
        return true;
    }
};

optional<vector<vector<Edge>>> kruskals(const vector<vector<Edge>>& g) {
    int n = (int)g.size();
    vector<tuple<int,int,int>> es;      // (weight, u, v)
    for (int u = 0; u < n; ++u)
        for (auto& e : g[u])
            if (u < e.to)
                es.push_back({e.weight, u, e.to});

    sort(es.begin(), es.end());

    UF uf(n);
    vector<vector<Edge>> mst(n);
    int added = 0;
    for (auto& [w, u, v] : es) {
        if (!uf.unite(u, v)) continue;
        mst[u].push_back({v, (int)w});
        mst[v].push_back({u, (int)w});
        if (++added == n - 1) break;        // tree complete
    }

    if (added != n - 1) return nullopt;     // disconnected
    return mst;
}
```

- Only take each undirected edge once (`u < e.to`). The adjacency list has both directions; without dedup, the sort doubles E and halves the effective threshold.
- Early break at `n-1` edges avoids processing the rest of the sorted list once the MST is complete.
- The Union-Find is self-contained: `find` with full path compression, `unite` by size (negative parent encodes size).

## Alternative -- Prim's algorithm

- Same result, different approach -- grows one tree from a root. O(V^2) for dense graphs beats Kruskal's O(E log E).
- See prims_list for the full version.

## Alternative -- Boruvka's algorithm (parallel-friendly)

- Add the cheapest edge from each component simultaneously, then recurse.
- Rarely coded by hand but parallelizes well -- used in some distributed MST implementations.

## Complexity

- Time: O(E log E) -- the sort dominates; the union-find work is amortized alpha(n) per op.
- Space: O(E) for the edge list, O(V) for the union-find and MST.

## Usage

- Any MST problem where edges arrive as a flat list rather than an adjacency structure.
- Hierarchical clustering (single-linkage = Kruskal stopped at k components).
- Network design: minimum cost to connect sites, cheapest way to lay cable between buildings.
- The "min cost to connect everything" question wherever it shows up.

## Cousins & contrasts

- **Prim's**: same result, different approach -- grows one tree from a root. O(V^2) for dense graphs beats Kruskal's O(E log E).
- **Union-Find**: the reason Kruskal is simple. Without it, cycle detection would need a full DFS per edge.
- **Boruvka's**: add the cheapest edge from each component simultaneously. Rarely coded but parallelizes well.
- **Reverse-delete MST**: sort descending and remove any edge that doesn't disconnect the graph -- O(E log E) and conceptually dual to Kruskal.
