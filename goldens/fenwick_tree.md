# Fenwick Tree (Binary Indexed Tree)

Prefix sums with point updates, both in O(log n) — a flat array impersonating a tree using only index arithmetic. Internally **1-indexed**.

## Intuition

- The magic is `lowbit(i) = i & -i`, the lowest set bit. Node `tree[i]` stores the aggregate of `(i - lowbit(i), i]` — a power-of-two block ending at `i`.
- Every index owns one power-of-two block, so any prefix `[1..i]` decomposes into ≤ log₂n blocks: strip bits (`i -= lowbit(i)`) until zero.
- Point update does the mirror walk: `i += lowbit(i)` visits every node whose range contains `i` — also ≤ log₂n of them.
- Complexity: O(log n) per op, O(n) space, tiny constants (two integer ops per level, no recursion).

## Struct: add / sum / length

```cpp
using namespace std;

struct Fenwick {
    vector<int> t;
    Fenwick(int n) : t(n + 1, 0) {}

    void add(int i, int d) {
        for (; i < (int)t.size(); i += i & -i) t[i] += d;
    }

    int sum(int i) {
        int s = 0;
        for (; i > 0; i -= i & -i) s += t[i];
        return s;
    }

    int length() { return (int)t.size() - 1; }
};
```

- Range sum `[l..r]` = `sum(r) - sum(l - 1)` because addition is invertible.
- Pitfall: **1-indexed** internally. `tree[0]` is never touched. Size the vector `n + 1` and shift external 0-based indices by one.
- Pitfall: supports only **invertible** prefix operations. Sum yes, min/max no — can't undo a min.

## Where it shows up

- Running totals and frequency counters with updates: order-book quantities, spreadsheet columns, histogram buckets.
- Counting inversions offline: compress values, walk right-to-left, sum = how many smaller seen.
- CP staple — "point update + prefix query".

## Cousins & contrasts

- **Segment tree**: handles arbitrary range queries and range updates with any associative op (min/max/gcd), but heavier — ~4n memory, recursion, bigger constant. Fenwick is the prefix-only ~2× faster special case.
- **Sqrt decomposition**: √n blocks, O(√n) ops — dumber but tolerates non-invertible ops and is easy to extend.
- **Plain prefix-sum array**: O(1) query, O(n) update — wins only on static data.
