# Bubble Sort

Sort ascending in place by repeatedly swapping adjacent out-of-order pairs — each pass "bubbles" the current maximum to its final slot at the right end. **Stable**, O(1) extra space, mostly pedagogical.

## Intuition

- One adjacent swap removes exactly one inversion, so total work is proportional to the inversion count — Θ(n²) worst and average.
- Loop invariant: after pass `i`, the last `i` elements are final; the inner loop only scans the unsorted prefix.
- Stability comes from swapping only on strict `>` — equal neighbors never change order.
- Early exit: a full pass with zero swaps means no inversions remain — already sorted. Nearly-sorted input finishes in O(n).

## Early-exit bubble sort

```cpp
using namespace std;

void sort(vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n; ++i) {
        bool swapped = false;
        for (int j = 0; j + 1 < n - i; ++j)
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        if (!swapped) break;
    }
}
```

- Inner bound `n - i`: never re-scan the bubbled tail.
- Keep comparison strict `>` — `>=` buys nothing and breaks stability.
- Reverse-sorted input still costs the full Θ(n²); the flag only helps when inversions are few.

## Where it shows up

- Teaching / interviews: the smallest complete "compare & swap neighbors" sort.
- Detecting sortedness: one flagged pass is literally an "is sorted?" check.
- Inversion intuition: a swap count is a lower bound on the inversion count.

## Cousins & contrasts

- **Insertion sort**: same O(n²) adaptive family, but shifts instead of swapping (half the writes) — strictly better in practice.
- **Selection sort**: also finalizes one element per pass, but locates the min with no swaps — O(n) writes total, never adaptive, unstable.
- **Cocktail shaker sort**: bubbles in both directions alternately; fixes the "small values crawl left" pathology, still Θ(n²).
- **Comparison-sort lower bound**: every comparison sort is Ω(n log n) worst-case — see merge_sort for the decision-tree argument; Θ(n²) sits far above it.
