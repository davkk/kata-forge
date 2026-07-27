# Find Min in Rotated Sorted Array

A sorted array of **distinct** values, rotated by an unknown offset: find the minimum in O(log n). The minimum is the single disorder point where the array "wraps".

## Intuition

- One rotation -> two sorted runs; everything in the left run exceeds everything in the right run, and the min is the head of the right run.
- Compare `a[mid]` with `a[hi]`: if `a[mid] > a[hi]`, mid sits in the left (larger) run -> the min is strictly right of mid. Otherwise mid is in the right run -> the min is at mid or left.
- Invariant: the min stays inside `[lo, hi]`; the window halves each step and converges on the answer.
- Returns `a[lo] == a[0]` (the original first element) when the array was never rotated.

## Approach -- halve against the right end

```cpp
using namespace std;

int find_min_rotated(const vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) lo = mid + 1;
        else                hi = mid;
    }
    return a[lo];
}
```

- `hi = mid`, never `mid - 1`: mid itself might be the minimum -- don't discard it. Symmetrically, `lo = mid + 1` is only safe in the `>` branch, where mid provably isn't the min.
- Already-sorted input: `a[mid] <= a[hi]` every time, so `hi` marches straight to 0 -- still O(log n), still correct.
- Comparing against `a[hi]` works because the right end always bounds the right run; comparing against `a[lo]` needs an extra is-sorted check.
- **Duplicates break the halving**: with `a[mid] == a[hi]` you can't tell which run mid is in -- the only fix (`--hi`) degrades the worst case to O(n).

## Alternative -- linear scan

- Walk once and track the running minimum. O(n) time, no rotation reasoning required.
- Use when the array is not guaranteed sorted+rotated, or when n is so small the O(log n) wins don't pay for the code complexity.

## Alternative -- search target in a rotated sorted array

- Find the pivot with this file's approach, then binary-search the correct run. O(log n) total.
- The single-pass fused version is also possible: after shrinking to the side that *might* contain the target, compare `a[lo] <= target <= a[mid]` to decide.

## Complexity

- Time: O(log n) for distinct elements; O(n) worst case with duplicates.
- Space: O(1).

## Usage

- Rotated logs / circular buffers: the minimum index is the logical start of the data.
- Stepping stone for "search target in rotated array": find the pivot here, then binary-search the correct run.
- Same predicate shape as "first bad version": one threshold, monotone on both sides.

## Cousins & contrasts

- **binary_search_list**: the pure version -- one monotone predicate, no rotation twist.
- **Search in rotated sorted array**: adds a target; either two passes (pivot, then search) or one fused pass with extra cases.
- **Find peak element**: also halves the window, but the predicate compares `a[mid]` vs `a[mid + 1]` instead of against an end.
- **Lower bound**: same half-open `[lo, hi)` discipline, different predicate -- the insertion-point variant.
