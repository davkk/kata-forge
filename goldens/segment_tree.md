# Segment Tree

Full binary tree over an array: leaves = elements, internal nodes = aggregate (sum/min/max) of their range. Range query and point update both O(log n).

## Intuition

- Each node owns segment `[lo, hi]`; children split at mid. A query range decomposes into ≤ **2 nodes per level**, so O(log n) nodes total.
- Point update walks one root-to-leaf path: change the leaf, recompute every ancestor on the way back up.
- Works with any associative op with an identity: sum (0), min (∞), max, gcd. Only the combine function changes.
- Complexity: build O(n), query/update O(log n), O(4n) array is the safe bound.

## Recursive struct (sum)

```cpp
using namespace std;

struct SegTree {
    vector<int> t;
    int n;

    SegTree(const vector<int>& a) : n((int)a.size()) {
        t.assign(4 * n, 0);
        build_(a, t, 1, 0, n - 1);
    }

    int query(int l, int r) { return query_(t, 1, 0, n - 1, l, r); }
    void update(int i, int val) { update_(t, 1, 0, n - 1, i, val); }
    int length() { return n; }
};

static void build_(const vector<int>& a, vector<int>& t, int v, int lo, int hi) {
    if (lo == hi) { t[v] = a[lo]; return; }
    int mid = lo + (hi - lo) / 2;
    build_(a, t, v*2, lo, mid);
    build_(a, t, v*2+1, mid+1, hi);
    t[v] = t[v*2] + t[v*2+1];
}

static int query_(const vector<int>& t, int v, int lo, int hi, int ql, int qr) {
    if (ql <= lo && hi <= qr) return t[v];
    if (hi < ql || lo > qr) return 0;
    int mid = lo + (hi - lo) / 2;
    return query_(t, v*2, lo, mid, ql, qr) + query_(t, v*2+1, mid+1, hi, ql, qr);
}

static void update_(vector<int>& t, int v, int lo, int hi, int i, int val) {
    if (lo == hi) { t[v] = val; return; }
    int mid = lo + (hi - lo) / 2;
    if (i <= mid) update_(t, v*2, lo, mid, i, val);
    else          update_(t, v*2+1, mid+1, hi, i, val);
    t[v] = t[v*2] + t[v*2+1];
}
```

- Nodes 1-indexed (root = 1, children `2v` / `2v+1`); keeps array small, arithmetic obvious.
- The `mid` split must be identical in all three functions — mismatched partitions are the classic index bug.
- Early exits: full containment returns immediately, full disjointness returns identity. Partial overlap recurses both ways.

## Alternative: lazy propagation (for range updates)

- To add x to a whole *range*, tag the node: apply `x * range_size` to `t[v]` now, record x in a parallel `lazy[v]`, don't touch children yet.
- Push down on demand: whenever a later query/update descends past `v`, flush `lazy[v]` into both children and clear it. Work stays O(log n) per range update instead of O(k log n).

## Where it shows up

- Range sum/min/max with updates: stock prices over time windows, leaderboard scores, sensor aggregates.
- CP staple — the default answer to "updates + range queries, op isn't invertible".
- Interval scheduling with counts: sweep events over coordinates, query/update coverage counts per segment.

## Cousins & contrasts

- **Fenwick tree**: prefix-only special case — ~n memory, iterative, ~2× faster constant — but needs invertible op (sum yes, min no) and can't do lazy range updates.
- **Sparse table**: static data only; O(n log n) build, O(1) RMQ via overlapping power-of-two intervals. No updates.
- **Sqrt decomposition**: √n blocks, O(√n) everything — crude but flexible and easy to improvise under pressure.
