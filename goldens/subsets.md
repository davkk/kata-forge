# Subsets (Power Set)

All 2^n subsets of a list of **distinct** integers -- including the empty set. The canonical first backtracking problem: a binary decision tree where every node is a valid answer.

## Intuition

- Shared skeleton: **choose -> explore -> unchoose** down a decision tree, one level per item.
- What changes per enumerator is the branching rule. Here: at each level, decide the **next item to append** from a start index onward. The start index never moves backward, so no subset repeats.
- Equivalent view: at each level, include the item or skip it. A binary tree of depth n has exactly **2^n leaves**, and every internal node is also a valid subset.
- Record `curr` at *every* node, not just leaves -- every prefix of choices is a subset.
- Output itself is 2^n subsets, so the work is O(n * 2^n) just to write the answer. The algorithm's overhead on top of that is linear.

## Approach 1 -- backtracking with start index

```cpp
using namespace std;

static void backtrack(const vector<int>& nums, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    out.push_back(curr);
    for (int i = start; i < (int)nums.size(); ++i) {
        curr.push_back(nums[i]);
        backtrack(nums, i + 1, curr, out);
        curr.pop_back();
    }
}

vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(nums, 0, curr, out);
    return out;
}
```

### Walkthrough

On `nums = [1, 2, 3]`:
- Call with start=0, curr=[]: record [], recurse
  - i=0, push 1: record [1], recurse
    - i=1, push 2: record [1,2], recurse
      - i=2, push 3: record [1,2,3] -> pop 3
    - pop 2
  - pop 1
  - i=1, push 2: record [2], recurse
    - i=2, push 3: record [2,3] -> pop 3
  - pop 2
  - i=2, push 3: record [3] -> pop 3
- return [[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]] -- all 2^3 = 8 subsets

- Push `curr` *before* the loop: the empty set comes for free.
- Pass `i + 1`, not `start + 1` -- the next level continues after the item just chosen.

## Complexity

- Time: O(n * 2^n) -- output dominates.
- Space: O(n) recursion depth plus O(n * 2^n) for the output itself.

## Approach 2 -- bitmask enumeration (n <= 20-ish)

```cpp
using namespace std;

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

- Every n-bit number maps to one subset: bit j set means item j included.
- No recursion, no bookkeeping; still O(n * 2^n). Wins when you want the simplest possible code and `n` fits in a machine word.
- For `n` around 20, this is the cleanest production form; for larger `n` you need the backtracking version to avoid the bitmask overflow.

## Alternative -- duplicate-safe version

- Sort first, then at each level skip values equal to the one just tried: `if (i > start && nums[i] == nums[i-1]) continue;`.
- Same skeleton, one extra guard, prevents the same subset from appearing with permuted duplicates.

## Usage

- Power-set enumeration: feature flags, test-case generation, exhaustive small-n search.
- Meet-in-the-middle precomputation: split n around 40 items, enumerate each half's 2^20 subsets, combine with binary search.
- Feature selection, subset-sum style problems, generating all states of a small system.

## Cousins & contrasts

- **Permutations**: same skeleton, branches over *all unused* items -- n! leaves instead of 2^n.
- **Combinations**: subsets of size exactly k -- same start-index walk, stop at depth k, C(n, k) leaves.
- **Subsets with duplicates**: sort first, skip equal values at the same level (same trick as duplicate permutations).
- **Iterative build-up**: start with `[[]]`, then for each element append it to every existing subset. Same result, no recursion.
