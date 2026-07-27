# Union-Find (Disjoint Set Union)

Tracks a partition of elements into disjoint sets. `find` locates a set's representative; `unite` merges two sets. Near-O(1) amortized -- inverse Ackermann alpha(n), <= 4 for any practical n.

## Intuition

- Each element starts as its own root. `parent[x] < 0` means x is a root; the absolute value is the set size.
- `find(x)` follows parent pointers to the root. **Path compression**: redirect every visited node directly to the root.
- `unite(a,b)`: find both roots, attach the smaller tree under the larger one (union by size). Keeps trees shallow.
- Without both optimizations, the tree can degrade to a linked list -- O(n) per operation.

## Approach -- compact implementation

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

- `find` is recursive with path compression -- the shortest correct form. Iterative avoids stack concerns but is longer.
- Always operate on roots: `find` both arguments of `unite` first. Never assign parent directly from non-root nodes.
- Negative parent = root + stores size: `-p[root]` = number of elements in the set. Useful for component-size queries.
- `unite` returns `true` if a merge happened. Useful in Kruskal for counting edges added.

## Alternative -- union by rank

- Rank = upper bound on tree height (instead of size). The same alpha(n) bound, slightly different constants.
- Size is more useful when you also need component-size queries; rank is the textbook variant.

## Alternative -- iterative find (no recursion)

- Two-pass form: walk up to root, then walk again setting parents to the root.
- Avoids stack overflow on deep pre-compression trees; the recursive form is shorter when trees stay shallow.

## Alternative -- path halving / splitting

- Halving: `p[x] = p[p[x]]; x = p[x];` -- compress during ascent by one level.
- Splitting: `int nxt = p[x]; p[x] = p[nxt]; x = nxt;` -- point every visited node directly at its grandparent.
- Same alpha(n) bound; splitting often benchmarks best, halving is the smallest code change.

## Complexity

- Time: O(alpha(n)) amortized per find / unite -- effectively constant for any practical n.
- Space: O(n) for the parent array.

## Usage

- Kruskal's MST (cycle detection): process edges in weight order, unite the endpoints, skip the edge if they are already connected.
- Dynamic connectivity: incremental land-cell addition (counting connected components as a map fills in), grouping records by equivalence.
- Percolation, image segmentation, offline connectivity queries (process deletions in reverse as unions).
- Finding the first edge in a list that creates a cycle in an otherwise-forest graph (the "extra edge" problem).

## Cousins & contrasts

- **DFS/BFS for connectivity**: O(V+E) per query. UF handles millions of unions/finds in near-constant time.
- **Union by rank vs size**: both achieve the same bound. Size is intuitive -- "how many elements does this root own?"
- **Without path compression**: `find` becomes O(log n). Works but loses near-O(1) guarantee.
- **Dynamic vs static**: UF supports only edge insertions (unions). For edge deletions, need a fully dynamic structure (Euler tour tree, divide-and-conquer offline).
