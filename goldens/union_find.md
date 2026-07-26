# Union-Find (Disjoint Set Union)

Tracks a partition of elements into disjoint sets. `find` locates a set's representative; `unite` merges two sets. Near-O(1) amortized — inverse Ackermann α(n), ≤ 4 for any practical n.

## Intuition

- Each element starts as its own root. `parent[x] < 0` means x is a root; the absolute value is the set size.
- `find(x)` follows parent pointers to the root. **Path compression**: redirect every visited node directly to the root.
- `unite(a,b)`: find both roots, attach the smaller tree under the larger one (union by size). Keeps trees shallow.
- Without both optimizations, the tree can degrade to a linked list — O(n) per operation.

## Compact implementation

```cpp
using namespace std;

struct UnionFind {
    vector<int> p;
    UnionFind(int n) : p(n, -1) {}

    int find(int x) {
        return p[x] < 0 ? x : (p[x] = find(p[x]));
    }

    bool unite(int a, int b) {          // C++ keyword: use 'unite' instead of 'union'
        a = find(a), b = find(b);
        if (a == b) return false;
        if (p[a] > p[b]) swap(a, b);    // a is the larger set (more negative)
        p[a] += p[b];
        p[b] = a;
        return true;
    }

    bool connected(int a, int b) { return find(a) == find(b); }
    int size() { return (int)p.size(); }
};
```

- `find` is recursive with path compression — the shortest correct form. Iterative avoids stack concerns but is longer.
- Always operate on roots: `find` both arguments of `unite` first. Never assign parent directly from non-root nodes.
- Negative parent = root + stores size: `-p[root]` = number of elements in the set. Useful for component-size queries.
- `unite` returns `true` if a merge happened. Useful in Kruskal for counting edges added.

### Optimization details

- **Why both are essential**: Path compression alone does not keep trees shallow — a chain of `unite` calls builds a tall tree, and compression only flattens paths traversed by `find`. Union by size alone guarantees O(log n) height. Together they achieve amortized α(n). Each node's parent changes at most O(log n) times across all operations; after that, path compression keeps it one hop from the root.

- **Iterative find (no recursion)**: Recursion risks stack overflow on deep pre-compression trees. Two-pass form — walk up to root, then walk again setting parents:

  ```cpp
  int find(int x) {
      int r = x;
      while (p[r] >= 0) r = p[r];
      while (p[x] >= 0) { int nxt = p[x]; p[x] = r; x = nxt; }
      return r;
  }
  ```

- **Path halving / splitting**: Alternatives that compress during ascent, avoiding a second pass. Halving: `p[x] = p[p[x]]; x = p[x];`. Splitting: `int nxt = p[x]; p[x] = p[nxt]; x = nxt;`. Same α(n) bound; splitting often benchmarks best.

- **Cache & memory**: If n fits in 16 bits, `int16_t` halves the `p` array cache footprint — matters in tight loops over millions of nodes. The single-vector layout (negative for root size, non-negative for parent) already minimizes indirection.

- **Payload merging**: When each set carries extra data (e.g., string list in accounts merge), merge the smaller set's payload into the larger set's. Same union-by-size principle applied to data — total moves stay O(n log n).

## Where it shows up

- Kruskal's MST (cycle detection), as above.
- Dynamic connectivity: "number of islands II" (adding land cells one by one), "accounts merge".
- Percolation, image segmentation, offline connectivity queries (process deletions in reverse as unions).
- Finding "redundant connection" (first edge connecting already-connected nodes in a graph).

## Cousins & contrasts

- **DFS/BFS for connectivity**: O(V+E) per query. UF handles millions of unions/finds in near-constant time.
- **Union by rank vs size**: both achieve the same bound. Size is intuitive — "how many elements does this root own?"
- **Without path compression**: `find` becomes O(log n). Works but loses near-O(1) guarantee.
- **Dynamic vs static**: UF supports only edge insertions (unions). For edge deletions, need a fully dynamic structure (Euler tour tree, divide-and-conquer offline).
