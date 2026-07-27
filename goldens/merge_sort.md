# Merge Sort

Divide & conquer: split the array in halves, sort each half recursively, then merge the two sorted halves with a temp buffer. **Θ(n log n) in all cases** and **stable**, at the cost of O(n) extra space.

## Intuition

- Merging two sorted runs is a two-pointer walk: repeatedly take the smaller head — O(n) for n elements.
- Recurrence T(n) = 2T(n/2) + Θ(n): log₂n levels of recursion, Θ(n) merge work per level ⇒ Θ(n log n), regardless of input.
- Take the **left** element on ties (`<=`) and equal elements keep their original order ⇒ stable.
- O(n) buffer + O(log n) recursion stack: the price of the guarantee is memory.
- Lower bound: any comparison sort is a decision tree with ≥ n! leaves, so minimum depth is log₂(n!) ≈ n log₂n. Merge sort matches the bound; Θ(n²) sorts (bubble, insertion, selection) sit far above it.

## Top-down recursive

```cpp
using namespace std;

void merge_halves(vector<int>& a, vector<int>& buf, int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi)
        buf[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < mid) buf[k++] = a[i++];
    while (j < hi)  buf[k++] = a[j++];
    for (i = lo; i < hi; ++i) a[i] = buf[i];
}

void msort(vector<int>& a, vector<int>& buf, int lo, int hi) {  // [lo, hi)
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    msort(a, buf, lo, mid);
    msort(a, buf, mid, hi);
    merge_halves(a, buf, lo, mid, hi);
}

void merge_sort(vector<int>& a) {
    vector<int> buf(a.size());
    msort(a, buf, 0, (int)a.size());
}
```

- Half-open `[lo, hi)` intervals: `mid` is a clean split point and the base case is `hi - lo <= 1`.
- Allocate the buffer once up front — per-call allocation wrecks the constant factor.
- The split phase does zero comparisons; all the work happens in the merge.

## Alternative: bottom-up (iterative)

- Merge runs of width 1, 2, 4, … pairwise; same Θ(n log n), no recursion stack, and the natural form for linked lists and external sorting.
- Natural merge sort (timsort's core) first scans for existing ascending runs — adaptive to pre-sorted data.

```cpp
void merge_sort(vector<int>& a) {
    vector<int> buf(a.size());
    int n = (int)a.size();
    for (int w = 1; w < n; w *= 2)
        for (int lo = 0; lo < n; lo += 2 * w)
            merge_halves(a, buf, lo, min(lo + w, n), min(lo + 2 * w, n));
}
```

## Where it shows up

- External sorting: files bigger than RAM — sort chunks in memory, then k-way merge runs from disk.
- `std::stable_sort`, and any standard library that must guarantee stability (Java's object sort, Python's timsort lineage).
- Counting inversions in O(n log n): count cross-half pairs during the merge step.
- Sorting linked lists: merging needs no random access (see cousins).

## Cousins & contrasts

- **Quick sort**: in-place and usually faster in practice, but unstable with an Θ(n²) worst case — merge sort trades memory for guarantees.
- **Linked-list merge sort**: same recursion, but the merge relinks nodes instead of copying — O(1) extra space, which is why it's the standard list sort.
- **Heap sort**: also Θ(n log n) worst case and in-place, but unstable with worse cache behavior.
- **Timsort**: merge sort + natural runs + insertion sort for short runs — the industrial-strength descendant.
