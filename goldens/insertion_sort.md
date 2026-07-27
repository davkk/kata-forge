# Insertion Sort

Sort in place by growing a sorted prefix: take the next element, shift larger predecessors one step right, drop it into the gap. **Stable** and **adaptive** — nearly-sorted input costs O(n).

## Intuition

- Invariant: `a[0..i)` is always sorted; inserting `a[i]` costs one comparison + shift per inversion it participates in.
- Adaptive: total work is O(n + inversions) — on sorted input the inner loop never iterates, giving O(n).
- Stability: shift only on strict `>`, so equal elements keep their original order.
- O(n²) worst (reverse-sorted), O(1) extra space, best-in-class constants for small n — sequential access, branch-light.

## Shift-and-insert

```cpp
using namespace std;

void insertion_sort(vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        int curr = a[i], j = i - 1;
        while (j >= 0 && a[j] > curr) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = curr;
    }
}
```

- Hoist `curr` out of the array: shifts are single assignments, not swaps — half the writes of bubble sort.
- The function is named `insertion_sort` to match the kata interface — stub generators expect that name.
- Condition order matters: `j >= 0 && a[j] > curr` — the short-circuit guards the index.
- Start at `i = 1`; a one-element prefix is trivially sorted.

## Alternative: binary insertion

- Locate the insertion point with binary search over the sorted prefix instead of scanning: comparisons drop from O(n²) to O(n log n).
- The shifts are still O(n²) total, so it only pays off when comparisons are expensive (long strings, costly comparators).

## Where it shows up

- Hybrid sorts: introsort (std::sort) and timsort switch to insertion sort for small runs (~16-64 elements) — nothing beats its constants at that scale.
- Sorting a hand of cards; online sorting as data arrives one item at a time.
- Nearly-sorted streams (logs with occasional out-of-order entries): O(n + k) for k inversions.

## Cousins & contrasts

- **Bubble sort**: same adjacent-inversion idea but swaps instead of shifts — 2× the writes, no compensating advantage.
- **Selection sort**: also grows a sorted prefix, but by selecting the min of the suffix — not adaptive, unstable.
- **Shell sort**: insertion sort over gapped subsequences; a good gap sequence upgrades it to roughly O(n^1.3).
