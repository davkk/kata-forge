# Subsets

Power set — all 2ⁿ subsets of a list of **distinct** ints. The canonical first backtracking problem.

## Intuition

- Shared skeleton: **choose → explore → unchoose** down a decision tree, one level per item. Append to `curr`, recurse, pop.
- What differs per enumerator is the branching rule. Here: at each level, decide the **next item to append** from a start index onward. The start index never moves backward, so no subset repeats.
- Equivalent: for each item, branch include/exclude — a binary tree of depth n, exactly **2ⁿ leaves**.
- Record `curr` at *every* node, not just leaves — every prefix of choices is a valid subset.
- Complexity: O(n·2ⁿ) time (output dominates), O(n) recursion depth.

## Backtracking with start index

```cpp
using namespace std;

static void backtrack(const vector<int>& nums, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    out.push_back(curr);
    for (int i = start; i < (int)nums.size(); ++i) {
        curr.push_back(nums[i]);           // choose
        backtrack(nums, i + 1, curr, out); // explore
        curr.pop_back();                   // unchoose
    }
}

vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(nums, 0, curr, out);
    return out;
}
```

- Push `curr` *before* the loop: the empty set comes for free.
- Pass `i + 1`, not `start + 1` — the next level continues after the item just chosen.

## Alternative: bitmask enumeration (n ≤ 20-ish)

- Every n-bit number maps to one subset: bit j set ⇔ item j included. Count from 0 to 2ⁿ−1.
- No recursion, no bookkeeping; still O(n·2ⁿ) — same as any power-set enumeration.

```cpp
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> out;
    int n = (int)nums.size();
    for (int mask = 0; mask < (1 << n); ++mask) {
        vector<int> curr;
        for (int j = 0; j < n; ++j)
            if (mask & (1 << j)) curr.push_back(nums[j]);
        out.push_back(curr);
    }
    return out;
}
```

## Where it shows up

- Power-set enumeration: feature flags, test-case generation, exhaustive small-n search.
- Meet-in-the-middle precomputation: split n ≈ 40 items, enumerate each half's 2²⁰ subsets, combine with binary search.
- Feature selection and subset-sum style problems.

## Cousins & contrasts

- **Permutations**: same skeleton, branches over *all unused* items — n! leaves instead of 2ⁿ.
- **Combinations**: subsets of size k — same start-index walk, stop at depth k, C(n,k) leaves.
- **Subsets with duplicates**: sort first, skip equal values at the same level (same trick as duplicate permutations).
