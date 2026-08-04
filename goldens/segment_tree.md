# Segment Tree -- cache every split range: a query touches <= 2 nodes per level, an update refolds one path
## Core idea
- Invariant: node `v` owns the segment `[lo, hi]` and `t[v]` aggregates it, folded from its two children; children split a parent at `mid`, so height stays log2(n).
- Mechanism: a query returns a whole cached node when fully inside, the identity when fully outside, recursing only on partial overlap; an update rewrites one leaf and refolds its root-to-leaf path.
## Build up
1. **Naive range sum**
```
sum: for (i = l; i <= r; i++) s += a[i];   // O(n) per query
```
2. **Split into a range tree**
```
t[v] = t[2v] + t[2v+1];   // node owns [lo, hi]; children split at mid
```
3. **Build leaves up**
```
if (lo == hi) t[v] = a[lo]; else { build children; t[v] = t[2v] + t[2v+1]; }
```
4. **Query: prune by overlap**
```
if (ql <= lo && hi <= qr) return t[v];     // fully inside
if (hi < ql || qr < lo) return 0;          // outside -> identity
```
5. **Update: refold one path**
```
if (lo == hi) t[v] = val; else { descend one child; t[v] = t[2v] + t[2v+1]; }
```
## Diagram
```
a = [1,3,5,7,9,11]
               [0..5] = 36
             /          \
        [0..2] = 9    [3..5] = 27
        /     \         /     \
   [0..1] = 4 [2]=5  [3..4]=16 [5]=11
    / \               / \
 [0]=1 [1]=3       [3]=7 [4]=9
query(1,4): [1]=3 + [2]=5 + [3..4]=16 = 24
update(2,10): 5 -> 10, refold [0..2] = 14, root = 41
```
## Approach -- recursive struct
```cpp
using namespace std;

class SegmentTree {
public:
    vector<int> t;
    int n;

    SegmentTree(vector<int> a) : n((int)a.size()) {
        t.assign(4 * n, 0);
        build_(a, 1, 0, n - 1);               // step 3
    }

    void build_(const vector<int>& a, int v, int lo, int hi) {
        if (lo == hi) { t[v] = a[lo]; return; }
        int mid = lo + (hi - lo) / 2;
        build_(a, v*2, lo, mid);
        build_(a, v*2+1, mid+1, hi);
        t[v] = t[v*2] + t[v*2+1];             // step 3: fold
    }

    int query_(int v, int lo, int hi, int ql, int qr) {
        if (ql <= lo && hi <= qr) return t[v];   // step 4: fully inside
        if (hi < ql || lo > qr) return 0;        // step 4: identity
        int mid = lo + (hi - lo) / 2;
        return query_(v*2, lo, mid, ql, qr) + query_(v*2+1, mid+1, hi, ql, qr);
    }

    void update_(int v, int lo, int hi, int i, int val) {
        if (lo == hi) { t[v] = val; return; }    // step 5: rewrite leaf
        int mid = lo + (hi - lo) / 2;
        if (i <= mid) update_(v*2, lo, mid, i, val); // step 5: descend one child
        else          update_(v*2+1, mid+1, hi, i, val);
        t[v] = t[v*2] + t[v*2+1];                // step 5: refold
    }

    int query(int left, int right) { return query_(1, 0, n - 1, left, right); }
    void update(int idx, int val) { update_(1, 0, n - 1, idx, val); }
    int size() { return n; }
};
```
- Root = 1, children `2v`/`2v+1` in a flat array sized `4n`; build, query, and update must all split at the same `mid` -- a mismatch is the classic index bug.
- The outside return must be the operation's identity (0 for sum), not a sentinel; `query(left, right)` is inclusive on both ends.
### Trace
- a = [1,3,5,7,9,11]: query(0,2) = cached 9; query(1,4) = 3 + 5 + 16 = 24; query(0,5) = root = 36.
- update(2,10): leaf 5 -> 10, refold [0..2] = 14, root = 41; query(2,2) = 10.
## Complexity
- Time: O(n) build; O(log n) per query and update. Space: O(n) as a 4n flat array.
## Alternative -- lazy propagation
- Range updates: tag a fully-covered node (`lazy[v] += x`, add `x * len` to `t[v]`) and flush tags only when a query descends past it -- range ops stay O(log n).
## Use when
- Reach for this when: point updates interleave with range queries and the op is associative but not invertible -- sum, min, max, gcd all plug in by swapping the combine.
- Queries are inclusive `[left, right]`; coverage sweeps and windowed aggregates over a changing array.
## Cousins
- **Fenwick tree**: prefix-only and needs invertibility, but ~n memory and ~2x faster.
- **Sparse table**: O(1) static range min, no updates.
- **Sqrt decomposition**: O(sqrt n) per op, tolerant of any aggregate.
