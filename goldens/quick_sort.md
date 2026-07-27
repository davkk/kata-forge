# Quick Sort

Pick a pivot, partition the array so everything <= pivot sits left of it, then recurse on both sides. **In-place** and the fastest comparison sort in practice on **average** -- but unstable, with an O(n^2) worst case.

## Intuition

- Partition invariant (Lomuto): `a[lo..store)` <= pivot, `a[store..i)` > pivot -- one forward scan maintains it, and the pivot lands exactly at its final sorted index.
- Average case: balanced-ish splits give log2(n) recursion levels * O(n) partition work -> O(n log n).
- Worst case: sorted input + last-element pivot -> maximally unbalanced splits, recursion depth n -> O(n^2) time and stack.
- In-place (O(log n) stack on average) but **unstable**: long-range swaps jump over equal elements.

## Approach 1 -- Lomuto partition (the simple form)

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

- `store` advances only on <= pivot: everything left of it is certified small, everything in `[store, i)` certified large.
- Both recursions exclude `p` -- the pivot is already placed for good.
- Closed `[lo, hi]` interval here; the base case is `lo >= hi`.

## Approach 2 -- Hoare's partition (fewer swaps)

- Two pointers `i` and `j` walk inward from both ends, swapping whenever `a[i] > pivot` meets `a[j] < pivot`. Stop when they cross.
- ~3x fewer swaps than Lomuto, but the boundary conditions are trickier (the returned index is the partition *point*, not the pivot's final index).
- Use it when swaps are expensive and you are sure about the off-by-one.

## Alternative -- better pivots and 3-way partition

- **Random pivot**: expected O(n log n) on any input -- no adversarial input can force the worst case.
- **Median-of-three**: pivot = median of first/middle/last element; kills the sorted-input worst case for free.
- **Dutch national flag**: partition into `< | = | >` in one scan; with many duplicates the equals drop out of the recursion entirely.
- **Hoare's partition**: two pointers walking inward; ~3x fewer swaps than Lomuto, trickier boundary conditions.

## Complexity

- Time: O(n log n) average, O(n^2) worst case.
- Space: O(log n) stack on average, O(n) in the worst case.

## Usage

- `std::sort` is introsort: quicksort, heapsort fallback past 2*log2(n) recursion depth, insertion sort for tiny ranges.
- Quickselect (kth smallest): partition once, recurse into one side only -- average O(n) selection.
- Cache-friendly sequential scans give it the best wall-clock constants of the O(n log n) family.
- The default general-purpose sort in most libraries when stability does not matter.

## Cousins & contrasts

- **Merge sort**: stable and guaranteed O(n log n) but needs an O(n) buffer -- see merge_sort for the Omega(n log n) comparison-sort lower bound.
- **Heap sort**: the other in-place O(n log n) option -- worst-case guaranteed, but slower constants and unstable too.
- **Quickselect**: same partition, one-sided recursion -- selection instead of a full sort.
- **Introsort**: starts as quicksort, falls back to heapsort when recursion gets too deep -- what `std::sort` actually does.
