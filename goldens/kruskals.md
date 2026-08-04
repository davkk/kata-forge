# Kruskal's MST -- cheapest edges first, union-find rejects the ones that close a cycle

## Core idea
- Cut property: the cheapest edge crossing any cut belongs to some MST, and processing edges cheapest-first finds it.
- Union-find answers "same component?" in near O(1), so the cycle test is near-free per edge.

## Build up
1. **Sort edges by weight**

```
sort(es.begin(), es.end());
```

2. **Union-find skips cycles**

```
if (!uf.unite(u, v)) continue;   // already connected -> would cycle
mst[u].push_back({v, w});
mst[v].push_back({u, w});
```

3. **n-1 edges finish**

```
if (++added == n - 1) break;
```

4. **Shortfall means disconnected**

```
if (added != n - 1) return nullopt;
```

## Diagram
```
sorted edge list (weight: u-v):
  1:0-2  1:1-4  1:3-6  1:5-6  2:4-5  3:0-1  4:1-2
  add    add    add    add    add    add    skip
  0-2 joins {0}{2}      5-6 joins {3,6}{5}
  1-4 joins {1}{4}      4-5 joins {1,4}{3,5,6}
  3-6 joins {3}{6}      0-1 joins {0,2}{1,3,4,5,6}
  4:1-2 skipped: union-find sees 1, 2 already in one tree
  n-1 = 6 edges -> stop.  MST total 1+1+1+1+2+3 = 9
```

## Approach -- sorted edges with union-find
```cpp
using namespace std;

class UF {
public:
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

    sort(es.begin(), es.end());         // step 1: cheapest edges first

    UF uf(n);
    vector<vector<Edge>> mst(n);
    int added = 0;
    for (auto& [w, u, v] : es) {
        if (!uf.unite(u, v)) continue;  // step 2: same tree -> would cycle
        mst[u].push_back({v, (int)w});
        mst[v].push_back({u, (int)w});
        if (++added == n - 1) break;    // step 3: tree complete
    }

    if (added != n - 1) return nullopt; // step 4: disconnected
    return mst;
}
```

- Step 2 is the add loop: union-find rejects same-component edges, both directions enter the MST, and `u < e.to` dedups the undirected list.
- Step 3's break skips the rest of the sort; step 4's check turns a shortfall into nullopt.

### Trace
- Picks 0-2, 1-4, 3-6, 5-6, 4-5, 0-1 in sorted order; 1-2 (weight 4) is skipped, 1 and 2 already joined. Total 9, all 7 nodes in one tree.

## Complexity
- O(E log E) time (the sort dominates; union-find is amortized near O(1)), O(E + V) space.

## Alternative -- Prim's
- Same MST grown from one root by cheapest cut edge, no global sort (see prims_list).
- Better when the graph arrives as a dense adjacency list.

## Use when
- Min-cost connection: reach for this when everything must be connected as cheaply as possible (cable layout, network design).
- Flat edge lists: reach for this when edges come pre-enumerated and sorting is natural; single-linkage clustering is Kruskal stopped at k components.

## Cousins
- Union-Find: the near-O(1) "same component?" tester the algorithm is built on.
- Reverse-delete: sort descending and drop edges that keep the graph connected; the dual.
- Boruvka: add every component's cheapest edge at once; parallel-friendly.
