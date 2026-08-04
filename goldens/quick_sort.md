# Quick Sort -- one partition parks one pivot: in-place O(n log n) average

## Core idea

- Invariant: after one Lomuto scan, `a[lo..store)` <= pivot and `a[store..hi)` > pivot; the final swap drops the pivot at its permanent sorted index.
- Mechanism: each partition fixes one element for good and splits the rest, so n elements take n partitions; balanced splits give O(n log n), lopsided splits O(n^2).

## Build up

1. **Partition around a pivot**

```
int pivot = a[hi], store = lo;
for (int i = lo; i < hi; ++i)
    if (a[i] <= pivot) swap(a[i], a[store++]);
```

2. **Pivot lands at its final index**

```
swap(a[store], a[hi]);
```

3. **Recurse both sides**

```
qs(a, lo, p - 1);
qs(a, p + 1, hi);
```

4. **Stop at one element**

```
if (lo >= hi) return;
```

## Diagram

```
partition [9 3 7 4 69 420 42], pivot = 42

9 3 7 4 | 69 420 42    9,3,7,4 <= pivot: swap with a[store]
        ^store   ^i    (self-swaps; store ends at 4)
9 3 7 4 | 69 420 42    69, 420 > pivot: store stays, i walks
        ^store   ^i
9 3 7 4 | 42 420 69    swap(a[store], a[hi]): pivot parked
        ^store ^pivot  at its final index 4
```

## Approach -- Lomuto partition

```cpp
using namespace std;

int lomuto(vector<int>& a, int lo, int hi) {   // closed [lo, hi]
    int pivot = a[hi];
    int store = lo;                            // step 1: boundary
    for (int i = lo; i < hi; ++i)
        if (a[i] <= pivot) swap(a[i], a[store++]);
    swap(a[store], a[hi]);                     // step 2: park pivot
    return store;
}

void qs(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;                      // step 4
    int p = lomuto(a, lo, hi);
    qs(a, lo, p - 1);                          // step 3
    qs(a, p + 1, hi);
}

void quick_sort(vector<int>& a) { qs(a, 0, (int)a.size() - 1); }
```

- `store` advances only on `a[i] <= pivot`, so the invariant holds by construction; step 2 is the payoff.
- Both recursions exclude `p`: the pivot needs no further work and the subproblems are strictly smaller.

### Trace

- [9,3,7,4,69,420,42]: pivot 42 -> index 4 -> [9,3,7,4,42,420,69]; left parks 4 at index 1 -> [3,4,7,9]; right parks 69 -> sorted.

## Complexity

- Time: O(n log n) average, O(n^2) worst (sorted input, naive pivot). Space: O(log n) stack average.

## Alternative -- Hoare partition

- Two pointers walk inward, swapping a too-big left element with a too-small right element: about 3x fewer swaps, but the returned index is a split point, not the pivot's final index.

## Use when

- Best in-place constants matter and stability is not required.
- Reach for this when "sort in place fast" or when you want the partition itself (quickselect, partition odds/evens).

## Cousins

- Merge sort: stable, guaranteed O(n log n), pays an O(n) buffer.
- Heapsort: the other in-place O(n log n) option, worst-case guaranteed, slower constants.
- Quickselect: the same partition with one-sided recursion -> O(n) selection.
