# Segment Tree

Full binary tree over an array: leaves = elements, internal nodes = aggregate (sum/min/max) of their range. Range query and point update both O(log n).

## Intuition

- Each node owns segment `[lo, hi]`; children split at mid. A query range decomposes into <= **2 nodes per level**, so O(log n) nodes total.
- Point update walks one root-to-leaf path: change the leaf, recompute every ancestor on the way back up.
- Works with any associative op with an identity: sum (0), min (infinity), max, gcd. Only the combine function changes.
- O(n) build, O(log n) query/update, O(4n) array is the safe bound.

## Approach 1 -- recursive struct (sum)

```cpp
using namespace std;

struct SegmentTree {
    vector<int> t;
    int n;

    SegmentTree(vector<int> a) : n((int)a.size()) {
        t.assign(4 * n, 0);
        build_(a, 1, 0, n - 1);
    }

    int query(int left, int right) { return query_(1, 0, n - 1, left, right); }
    void update(int idx, int val) { update_(1, 0, n - 1, idx, val); }
    int length() { return n; }
};

void build_(const vector<int>& a, vector<int>& t, int v, int lo, int hi) {
    if (lo == hi) { t[v] = a[lo]; return; }
    int mid = lo + (hi - lo) / 2;
    build_(a, t, v*2, lo, mid);
    build_(a, t, v*2+1, mid+1, hi);
    t[v] = t[v*2] + t[v*2+1];
}

int query_(const vector<int>& t, int v, int lo, int hi, int ql, int qr) {
    if (ql <= lo && hi <= qr) return t[v];
    if (hi < ql || lo > qr) return 0;
    int mid = lo + (hi - lo) / 2;
    return query_(t, v*2, lo, mid, ql, qr) + query_(t, v*2+1, mid+1, hi, ql, qr);
}

void update_(vector<int>& t, int v, int lo, int hi, int i, int val) {
    if (lo == hi) { t[v] = val; return; }
    int mid = lo + (hi - lo) / 2;
    if (i <= mid) update_(t, v*2, lo, mid, i, val);
    else          update_(t, v*2+1, mid+1, hi, i, val);
    t[v] = t[v*2] + t[v*2+1];
}
```

- Nodes 1-indexed (root = 1, children `2v` / `2v+1`); keeps array small, arithmetic obvious.
- The `mid` split must be identical in all three functions -- mismatched partitions are the classic index bug.
- Early exits: full containment returns immediately, full disjointness returns identity. Partial overlap recurses both ways.

## Approach 2 -- iterative bottom-up

- Build leaves at indices `[n, 2n)`, internal nodes at `[1, n)`. Parent = `i/2`, children = `2i` and `2i+1`.
- Query walks up two chains (one for the left bound, one for the right) and combines; update walks from a leaf to the root.
- Faster constants than the recursive form, no stack depth, and easier to extend with lazy propagation.

## Alternative -- lazy propagation (for range updates)

- To add x to a whole *range*, tag the node: apply `x * range_size` to `t[v]` now, record x in a parallel `lazy[v]`, don't touch children yet.
- Push down on demand: whenever a later query/update descends past `v`, flush `lazy[v]` into both children and clear it. Work stays O(log n) per range update instead of O(k log n).

## Alternative -- sparse segment tree (only touched ranges)

- Use a hash map (`unordered_map`) for the tree nodes; only allocate nodes that are actually visited.
- O(K log N) memory where K is the number of point updates; perfect for offline problems with huge coordinate ranges and few updates.

## Complexity

- Time: O(n) build, O(log n) query and update.
- Space: O(n), or O(4n) for the safe array bound.

## Usage

- Range sum/min/max with updates: stock prices over time windows, leaderboard scores, sensor aggregates.
- The default answer to "updates + range queries, op isn't associative-with-inverse" in competitive programming.
- Interval scheduling with counts: sweep events over coordinates, query/update coverage counts per segment.
- Any "what is the aggregate over this index range, where values change over time" question.

## Cousins & contrasts

- **Fenwick tree**: prefix-only special case -- ~n memory, iterative, ~2x faster constant -- but needs invertible op (sum yes, min no) and can't do lazy range updates.
- **Sparse table**: static data only; O(n log n) build, O(1) RMQ via overlapping power-of-two intervals. No updates.
- **Sqrt decomposition**: sqrt(n) blocks, O(sqrt(n)) everything -- crude but flexible and easy to improvise under pressure.
- **Lazy propagation**: the range-update extension; turns the segment tree into a full interval-arithmetic engine.
