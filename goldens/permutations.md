# Permutations

All n! orderings of a list of **distinct** numbers. Backtracking where the branching rule is "any unused item may go next".

## Intuition

- Shared skeleton: **choose -> explore -> unchoose** down a decision tree. Position `k` is the level; try every still-unused item at that position -- a tree of degree n, n-1, ... -> **n! leaves**.
- The swap variant tracks "unused" in place: elements left of `k` are fixed, elements at `k..n-1` are the candidate pool. Swap a candidate into `k`, recurse, swap back.
- Every permutation is generated exactly once because each path fixes a distinct position with a distinct value.
- Output is n! permutations of n elements, so the work is O(n * n!) just to write the answer.

## Approach 1 -- backtracking with in-place swap

```cpp
using namespace std;

static void backtrack(vector<int>& a, int k, vector<vector<int>>& out) {
    if (k == (int)a.size()) {
        out.push_back(a);
        return;
    }
    for (int i = k; i < (int)a.size(); ++i) {
        swap(a[k], a[i]);
        backtrack(a, k + 1, out);
        swap(a[k], a[i]);
    }
}

vector<vector<int>> permutations(const vector<int>& nums) {
    vector<vector<int>> out;
    vector<int> a = nums;
    backtrack(a, 0, out);
    return out;
}
```

### Walkthrough

On `nums = [1, 2, 3]` (swap variant, array shown after each swap):
- k=0, a=[1,2,3]: try i=0 (no swap), recurse with k=1
  - k=1, a=[1,2,3]: i=1 (no swap) -> k=2
    - k=2, a=[1,2,3]: i=2 -> record [1,2,3]
  - i=2: swap a[1],a[2] -> a=[1,3,2], recurse, record [1,3,2], swap back -> a=[1,2,3]
- i=1: swap a[0],a[1] -> a=[2,1,3], recurse with k=1
  - k=1, a=[2,1,3]: i=1 (no swap) -> record [2,1,3]
  - i=2: swap -> a=[2,3,1], record [2,3,1]
- i=2: swap a[0],a[2] -> a=[3,2,1], recurse -> record [3,2,1] and [3,1,2]
- return all 3! = 6 permutations: [1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,2,1], [3,1,2]

- The undo swap is mandatory: the pool at `k..n-1` must look identical before every loop iteration.
- A `used[]` boolean plus a build-list works too; the swap version just saves the extra array.

## Complexity

- Time: O(n * n!) -- output dominates.
- Space: O(n) for the swap version, O(n) extra for `used[]` in the flag version.

## Approach 2 -- backtracking with a `used` flag

```cpp
using namespace std;

static void go(const vector<int>& nums, vector<bool>& used,
               vector<int>& curr, vector<vector<int>>& out) {
    if ((int)curr.size() == (int)nums.size()) {
        out.push_back(curr);
        return;
    }
    for (int i = 0; i < (int)nums.size(); ++i) {
        if (used[i]) continue;
        used[i] = true;
        curr.push_back(nums[i]);
        go(nums, used, curr, out);
        curr.pop_back();
        used[i] = false;
    }
}

vector<vector<int>> permutations(const vector<int>& nums) {
    vector<vector<int>> out;
    vector<int> curr;
    vector<bool> used(nums.size(), false);
    go(nums, used, curr, out);
    return out;
}
```

- More array overhead, but reads more naturally as the "include/exclude each item" form.
- Easier to extend to duplicate handling: sort first, then `if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;`.

## Alternative -- `std::next_permutation` (lexicographic order)

- On a sorted array, `do { ... } while (next_permutation(a.begin(), a.end()));` walks all n! orderings in lexicographic order.
- Zero recursion, minimal code, but requires the input sorted first and produces output in a specific order. Ideal when the kata needs ordered enumeration.
- Heap's algorithm generates all n! permutations with exactly one swap per permutation -- minimal work but trickier to recall.

## Usage

- Brute-force small cases: TSP with <= 10 cities, assignment problems, anagram solvers.
- Scheduling and ordering search where feasibility is checked per ordering.
- Base generator for shuffle testing, permutation-based heuristics, and exhaustive small-n search.

## Cousins & contrasts

- **Subsets**: same skeleton, each item branches include/exclude -- 2^n leaves instead of n!.
- **Combinations**: start-index walk stopped at depth k -- C(n, k) leaves.
- **Permutations with duplicates**: sort first, skip equal values at the same level (same trick as duplicate subsets).
- **Lexicographic successor**: `next_permutation` is the right tool when sorted order matters.
