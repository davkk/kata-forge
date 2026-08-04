# Merge Sort -- split is free, merging does the work: O(n log n) on every input

## Core idea

- Invariant: sorting [lo, hi) reduces to sorting [lo, mid) and [mid, hi), then one two-pointer pass merges two sorted runs; a run of size 1 is already sorted.
- Mechanism: the split makes no comparisons, so T(n) = 2T(n/2) + O(n) resolves to O(n log n) always, stable, at the cost of an O(n) buffer.

## Build up

1. **Merge two sorted runs**

```
if (a[i] <= a[j]) buf[k++] = a[i++];
else              buf[k++] = a[j++];
```

2. **One pass, two pointers**

```
while (i < mid) buf[k++] = a[i++]; while (j < hi) buf[k++] = a[j++];
for (i = lo; i < hi; ++i) a[i] = buf[i];
```

3. **Split until runs are size 1**

```
if (hi - lo <= 1) return;            // one element: sorted
int mid = lo + (hi - lo) / 2;
```

4. **Recursion = the split**

```
msort(a, buf, lo, mid);
msort(a, buf, mid, hi);
merge_halves(a, buf, lo, mid, hi);
```

## Diagram

```
      [5 2 4 6 1 3]
      /           \
 [5 2 4]         [6 1 3]    split: pick mid, no compares
  /  \            /  \
[5] [2 4]      [6] [1 3]
     / \            / \
   [2] [4]        [1] [3]   runs of size 1, trivially sorted
   merge up via buf: [2 4 5] + [1 3 6] -> [1 2 3 4 5 6]
```

## Approach -- top-down recursive

```cpp
using namespace std;

void merge_halves(vector<int>& a, vector<int>& buf, int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi)          // step 1: compare heads
        buf[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < mid) buf[k++] = a[i++]; // step 2: drain + copy back
    while (j < hi)  buf[k++] = a[j++];
    for (i = lo; i < hi; ++i) a[i] = buf[i];
}

void msort(vector<int>& a, vector<int>& buf, int lo, int hi) { // [lo, hi)
    if (hi - lo <= 1) return;          // step 3
    int mid = lo + (hi - lo) / 2;
    msort(a, buf, lo, mid);            // step 4
    msort(a, buf, mid, hi);
    merge_halves(a, buf, lo, mid, hi); // steps 1-2
}

void merge_sort(vector<int>& a) {
    vector<int> buf(a.size());         // one buffer, threaded through
    msort(a, buf, 0, (int)a.size());
}
```

- The build steps in order: compare heads, drain and copy back, size-1 base, recurse then merge.
- `a[i] <= a[j]` (not `<`) keeps equal elements in arrival order -> stable.

### Trace

- [5,2,4,6,1,3]: split to singletons; [2,4]+[5] -> [2,4,5]; [1,3]+[6] -> [1,3,6]; top merge emits 1,2,3,4,5 then drains 6.

## Complexity

- Time: O(n log n) best, average, worst. Space: O(n) buffer + O(log n) recursion stack.

## Alternative -- bottom-up iterative

- Merge runs of width 1, 2, 4, ... with the same merge_halves: no recursion stack, natural for linked lists and external sorting.

## Use when

- Need a worst-case guarantee AND stability, and can afford O(n) extra memory.
- Reach for this when "sort but never worse than O(n log n)" or when counting inversions (tally cross-half pairs during the merge).

## Cousins

- Quick sort: in-place, faster constants, but O(n^2) worst and unstable.
- Linked-list merge sort: same split and merge, relinking nodes instead of a buffer.
- Heapsort: in-place O(n log n) worst case, unstable, worse cache behavior.
