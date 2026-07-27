# Quick Sort

Pick a pivot, partition the array so everything ≤ pivot sits left of it, then recurse on both sides. **In-place** and the fastest comparison sort in practice on **average** — but unstable, with an Θ(n²) worst case.

## Intuition

- Partition invariant (Lomuto): `a[lo..store)` ≤ pivot, `a[store..i)` > pivot — one forward scan maintains it, and the pivot lands exactly at its final sorted index.
- Average case: balanced-ish splits give log₂n recursion levels × O(n) partition work ⇒ Θ(n log n).
- Worst case: sorted input + last-element pivot ⇒ maximally unbalanced splits, recursion depth n ⇒ Θ(n²) time and stack.
- In-place (O(log n) stack on average) but **unstable**: long-range swaps jump over equal elements.

## Lomuto partition

```cpp
using namespace std;

int lomuto(vector<int>& a, int lo, int hi) {   // closed [lo, hi]
    int pivot = a[hi];
    int store = lo;                              // a[lo..store) <= pivot
    for (int i = lo; i < hi; ++i)
        if (a[i] <= pivot) swap(a[i], a[store++]);
    swap(a[store], a[hi]);                       // pivot to its final index
    return store;
}

void qs(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;                        // 0/1 elements
    int p = lomuto(a, lo, hi);
    qs(a, lo, p - 1);
    qs(a, p + 1, hi);
}

void quick_sort(vector<int>& a) { qs(a, 0, (int)a.size() - 1); }
```

- `store` advances only on ≤ pivot: everything left of it is certified small, everything in `[store, i)` certified large.
- Both recursions exclude `p` — the pivot is already placed for good.
- Closed `[lo, hi]` interval here; the base case is `lo >= hi`.

## Alternative: better pivots & 3-way partition

- **Random pivot**: expected Θ(n log n) on any input — no adversarial input can force the worst case.
- **Median-of-three**: pivot = median of first/middle/last element; kills the sorted-input worst case for free.
- **Dutch national flag**: partition into `< | = | >` in one scan; with many duplicates the equals drop out of the recursion entirely.
- **Hoare's partition**: two pointers walking inward; ~3× fewer swaps than Lomuto, trickier boundary conditions.

## Where it shows up

- `std::sort` is introsort: quicksort, heapsort fallback past 2·log₂n recursion depth, insertion sort for tiny ranges.
- Quickselect (kth smallest): partition once, recurse into one side only — average O(n) selection.
- Cache-friendly sequential scans give it the best wall-clock constants of the Θ(n log n) family.

## Cousins & contrasts

- **Merge sort**: stable and guaranteed Θ(n log n) but needs an O(n) buffer — see merge_sort for the Ω(n log n) comparison-sort lower bound.
- **Heap sort**: the other in-place Θ(n log n) option — worst-case guaranteed, but slower constants and unstable too.
- **Quickselect**: same partition, one-sided recursion — selection instead of a full sort.
