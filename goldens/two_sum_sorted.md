# Two Sum (Sorted Array)

Find the 1-indexed pair of indices whose values sum to a target in a **sorted** array. Sortedness buys O(n) time and O(1) space via two pointers at opposite ends.

## Intuition

- Put `l` at the smallest element, `r` at the largest. Their sum brackets the target: too small → advance `l`; too big → retreat `r`.
- Why no pair is ever skipped: if `a[l] + a[r] < target`, then `a[l]` is too small against the *largest* available partner — so it's too small against everything and can be discarded forever. Symmetric argument for `a[r]` when the sum is too large.
- Each step eliminates one index, so at most n steps: O(n) time, O(1) space.

## Two pointers

```cpp
using namespace std;

vector<int> two_sum(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == target) return {l + 1, r + 1};  // 1-indexed
        if (sum < target) ++l;                     // need a bigger left
        else              --r;                     // need a smaller right
    }
    return {-1, -1};
}
```

- Return `{l + 1, r + 1}` — 1-indexed per the problem contract.
- The discards are justified only by sortedness; on unsorted input the same moves would skip valid pairs.
- Termination guaranteed by `l < r` — pointers cross only when no pair remains.

## Where it shows up

- Budget/pairing queries on sorted data: two prices summing to a budget, two trades netting to zero.
- Inner loop of 3Sum / 4Sum: fix one element, two-pointer the rest.
- The squeeze-from-both-ends pattern behind pair-with-difference and container-with-most-water problems.

## Cousins & contrasts

- **Two sum (unsorted)**: hash map value → index, one O(n) pass but O(n) space; sorting first costs O(n log n) and enables this file's O(1)-space version.
- **3Sum**: sort, fix each element, run this two-pointer scan on the remainder — O(n²) total.
- **Binary search per element**: for each `a[i]` search `target - a[i]`; O(n log n), more moving parts, dominated by two pointers once sorted.
- **Sliding window**: same two indices, but both move right — for windowed predicates, not pair sums.
