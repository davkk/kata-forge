# Fenwick Tree (Binary Indexed Tree)

Prefix sums with point updates, both in O(log n) -- a flat array impersonating a tree using only index arithmetic. Internally **1-indexed**.

## Intuition

- The magic is `lowbit(i) = i & -i`, the lowest set bit. Node `tree[i]` stores the aggregate of `(i - lowbit(i), i]` -- a power-of-two block ending at `i`.
- Every index owns one power-of-two block, so any prefix `[1..i]` decomposes into <= log2(n) blocks: strip bits (`i -= lowbit(i)`) until zero.
- Point update does the mirror walk: `i += lowbit(i)` visits every node whose range contains `i` -- also <= log2(n) of them.
- O(log n) per op, O(n) space, tiny constants (two integer ops per level, no recursion).

## Approach -- struct with add / sum / length

```cpp
using namespace std;

struct FenwickTree {
    vector<int> t;

    FenwickTree(int n) : t(n + 1, 0) {}

    void add(int idx, int delta) {
        for (; idx < (int)t.size(); idx += idx & -idx) t[idx] += delta;
    }

    int sum(int idx) {
        int s = 0;
        for (; idx > 0; idx -= idx & -idx) s += t[idx];
        return s;
    }

    int length() { return (int)t.size() - 1; }
};
```

- Range sum `[l..r]` = `sum(r) - sum(l - 1)` because addition is invertible.
- Pitfall: **1-indexed** internally. `tree[0]` is never touched. Size the vector `n + 1` and shift external 0-based indices by one.
- Pitfall: supports only **invertible** prefix operations. Sum yes, min/max no -- can't undo a min.

## Alternative -- segment tree (when the op is non-invertible)

- Same use case (point update + range query) but supports any associative operation including min, max, gcd.
- ~4n memory vs ~n, iterative vs recursive, ~2x slower constants. The Fenwick is the prefix-only win.

## Alternative -- sqrt decomposition (dumber but flexible)

- Split the array into blocks of ~sqrt(n); keep a per-block aggregate. Updates touch one block, queries touch O(sqrt(n)) blocks.
- O(sqrt(n)) per op, no associativity assumption, trivial to extend with custom aggregates. The right answer when the operation is non-associative or you need to improvise under pressure.

## Alternative -- plain prefix-sum array (static data)

- O(1) query, O(n) update. If nothing ever changes after build, a plain prefix-sum array beats the Fenwick on both axes.

## Complexity

- Time: O(log n) per add and sum.
- Space: O(n).

## Usage

- Running totals and frequency counters with updates: order-book quantities, spreadsheet columns, histogram buckets.
- Counting inversions offline: compress values, walk right-to-left, sum = how many smaller seen.
- The "point update + prefix query" staple of competitive programming and financial data.
- Database query optimizers use the same idea for indexed range aggregates.

## Cousins & contrasts

- **Segment tree**: handles arbitrary range queries and range updates with any associative op (min/max/gcd), but heavier -- ~4n memory, recursion, bigger constant. Fenwick is the prefix-only ~2x faster special case.
- **Sqrt decomposition**: sqrt(n) blocks, O(sqrt(n)) ops -- dumber but tolerates non-invertible ops and is easy to extend.
- **Plain prefix-sum array**: O(1) query, O(n) update -- wins only on static data.
- **Sparse Fenwick (offline queries)**: hash the indices that actually appear, build the tree over the compressed set. O(K log K) where K is the number of distinct indices.
