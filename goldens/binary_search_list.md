# Binary Search

Find a target in a **sorted** array by halving the search interval every step -- O(log n) instead of the O(n) of a linear scan.

## Intuition

- Sortedness means one comparison kills half the candidates: if `a[mid] < target`, everything at or left of `mid` is dead.
- Keep the invariant: *if the target exists, it is inside `[lo, hi)`*. Every iteration shrinks that window and never discards the answer.
- Use the half-open interval `[lo, hi)`: update `hi = mid` (mid already checked) and `lo = mid + 1`. This convention removes the classic +/-1 off-by-one bugs.
- Why O(log n): n -> n/2 -> n/4 -> ... -> 1 takes exactly log2(n) steps. 10^9 elements ~= 30 comparisons.

## Approach 1 -- presence check

```cpp
using namespace std;

bool search(const vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size();        // [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;      // same as (lo+hi)/2, no overflow
        if (a[mid] == target) return true;
        if (a[mid] < target) lo = mid + 1; // keep right half
        else                 hi = mid;     // keep left half
    }
    return false;
}
```

- `mid = lo + (hi - lo) / 2`, never `(lo + hi) / 2` -- the latter overflows for large indices.
- The loop needs nothing but a monotone predicate: "is `a[i]` still too small?"

## Approach 2 -- lower bound (insertion point)

```cpp
int lower_bound_(const vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < target) lo = mid + 1;
        else                 hi = mid;
    }
    return lo; // first index with a[i] >= target (== a.size() if none)
}
```

- Return the first index where `a[i] >= target` instead of a bool; presence check becomes `idx < n && a[idx] == target`.
- Same loop, no early exit on equality -- always shrink left on `a[mid] >= target`.
- This is exactly `std::lower_bound`, and the form you want for "first element >= x" or "count occurrences" problems.

## Alternative -- binary search on the answer

- "Minimum ship capacity to make D days", "smallest k such that predicate(k) is true" -- the feasibility predicate is monotone, so search the answer space, not the input.
- Same code, applied to `[lo, hi] = [min_possible_answer, max_possible_answer]` instead of array indices.
- This is the trick that turns "find the threshold" problems into O(log n).

## Complexity

- Time: O(log n).
- Space: O(1).

## Usage

- **Binary search on the answer**: the feasibility predicate is monotone, so search the answer space, not the input.
- Rotated sorted arrays (see find_min_rotated_sorted), sorted 2D matrices, integer square root.
- Any O(n log n) solution that sorts first, then answers queries by lookup.
- Library membership and prefix queries in `std::set` / `std::map` (the tree form of the same idea).

## Cousins & contrasts

- **Linear search**: works on unsorted data, O(n). Sortedness is the price of admission for O(log n).
- **Binary search tree**: the same idea as a data structure -- each comparison walks one edge -- but O(log n) only holds if the tree stays balanced.
- **Two pointers**: also shrinks a window, but from both ends at once; use for pair/triplet predicates, not single-target lookup.
- **`std::lower_bound` / `std::upper_bound`**: the library versions of approach 2; reach for them when the type is fully ordered and you have iterators.
