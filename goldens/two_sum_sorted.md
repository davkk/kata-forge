# Two Sum (Sorted)

Find the 1-indexed pair of indices whose values sum to a target in a **sorted** array. Sortedness unlocks the optimal O(n) two-pointer scan.

## Intuition

- Place `l` at the smallest element and `r` at the largest. Their sum brackets the target: too small -> push `l` right, too big -> pull `r` left.
- A discard is safe only because the input is sorted: if `a[l] + a[r] < target`, then `a[l]` is too small against the *largest* available partner, so it is too small against *every* partner and can be abandoned. Symmetric for `a[r]` when the sum is too large.
- Each iteration removes one index, so at most n steps. `l < r` guarantees termination -- pointers cross only when no pair remains.

## Approach -- two pointers

```cpp
using namespace std;

vector<int> two_sum(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == target) return {l + 1, r + 1};
        if (sum < target) ++l;
        else              --r;
    }
    return {-1, -1};
}
```

### Walkthrough

On `a = [2, 7, 11, 15]`, `target = 9`:
- l=0, r=3: 2+15=17 > 9 -> r-- -> (0, 2)
- l=0, r=2: 2+11=13 > 9 -> r-- -> (0, 1)
- l=0, r=1: 2+7=9 == 9 -> return {1, 2}

- Return `{l + 1, r + 1}` because the problem contract is 1-indexed.
- The discards depend entirely on sortedness; the same moves on an unsorted array would skip valid pairs.

## Complexity

- Time: O(n) -- each index is visited at most once.
- Space: O(1) -- only the two pointers.

## Alternative -- hash map (unsorted input)

- Walk once, storing each value's index in `unordered_map<int, int>`. For each `a[i]`, check if `target - a[i]` is already in the map -- first hit gives the pair.
- O(n) time with O(n) space, no sorting required. This is the general-purpose form; the two-pointer version above wins whenever the input is already sorted or sorting is acceptable.
- A third option -- sort the input first, then two-pointer -- costs O(n log n) and changes indices (the original positions are lost unless you keep a parallel index array).

## Usage

- Pair-summing queries on sorted data: prices hitting a budget, trade offsets netting to zero.
- The inner loop of 3-element and 4-element sum katas: fix one element, two-pointer the rest.
- The squeeze-from-both-ends pattern behind container-with-most-water and pair-with-difference problems.
- Two-sum itself is the prototype of "sortedness buys both time and space".

## Cousins & contrasts

- **Two sum on an unsorted array**: hash map value to index, one O(n) pass but O(n) space -- the alternative above.
- **Binary search per element**: for each `a[i]` search `target - a[i]` in O(log n); total O(n log n). Always dominated by the two-pointer scan once the input is sorted.
- **3-element sum katas**: sort, fix one element, run this two-pointer scan on the remainder -- O(n^2) total.
- **Sliding window**: also two indices, but both move right -- for windowed predicates, not pair sums.
