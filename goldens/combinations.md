# Combinations

All C(n, k) k-element subsets of `{1, 2, ..., n}` -- subsets of size exactly k, order irrelevant. The "subsets, but only depth k" enumerator.

## Intuition

- Shared skeleton: **choose -> explore -> unchoose** down a decision tree. The branching rule: pick the **next item from a start index onward** (like subsets), but stop once `curr` reaches size k.
- The start index guarantees increasing order inside every combination, so `{1, 3}` appears once and `{3, 1}` never exists.
- Leaves sit at depth k -> exactly **C(n, k) = n! / (k! * (n-k)!)** leaves.
- Prune: if `curr.size() + (n - i + 1) < k`, the remaining items cannot fill k -- break the loop. Keeps work at O(C(n, k)) instead of exploring doomed branches.
- Output is C(n, k) combinations of k elements, so the work is O(k * C(n, k)) just to write the answer.

## Approach 1 -- backtracking with start index and size cap

```cpp
using namespace std;

static void backtrack(int n, int k, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    if ((int)curr.size() == k) {
        out.push_back(curr);
        return;
    }
    for (int i = start; i <= n; ++i) {
        if ((int)curr.size() + (n - i + 1) < k) break;
        curr.push_back(i);
        backtrack(n, k, i + 1, curr, out);
        curr.pop_back();
    }
}

vector<vector<int>> combinations(int n, int k) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(n, k, 1, curr, out);
    return out;
}
```

### Walkthrough

`combinations(4, 2)` should produce C(4, 2) = 6 results:
- backtrack(n=4, k=2, start=1, curr=[]):
  - curr.size=0 + (4-1+1)=4 >= 2 -> try i=1: push 1
    - start=2, curr=[1]: try i=2: push 2 -> record [1,2]; pop
    - i=3: push 3 -> record [1,3]; pop
    - i=4: push 4 -> record [1,4]; pop
  - pop 1
  - try i=2: push 2
    - i=3: push 3 -> record [2,3]; pop
    - i=4: push 4 -> record [2,4]; pop
  - i=3: push 3
    - i=4: 1 + (4-4+1)=2 >= 2 -> push 4 -> record [3,4]; pop
  - (i=4 would not fit since curr=[3] has 1 slot left and only 1 value remains)
- return [[1,2], [1,3], [1,4], [2,3], [2,4], [3,4]]

- `k == 0` yields one empty combination -- falls out of the base case naturally.
- Pass `i + 1`, not `start + 1`; loop bound is `<= n` since values are `1..n`.
- The prune is a `break`, not `continue`: as `i` grows, the remaining count only shrinks.

## Complexity

- Time: O(k * C(n, k)) -- output copies dominate.
- Space: O(k) recursion depth plus output.

## Approach 2 -- bitmask enumeration (small n)

```cpp
using namespace std;

vector<vector<int>> combinations(int n, int k) {
    vector<vector<int>> out;
    for (int mask = 0; mask < (1 << n); ++mask) {
        if (__builtin_popcount(mask) != k) continue;
        vector<int> curr;
        for (int j = 0; j < n; ++j)
            if (mask & (1 << j)) curr.push_back(j + 1);
        out.push_back(curr);
    }
    return out;
}
```

- Iterate every n-bit mask, keep the ones with exactly k bits set, decode to the combination.
- No recursion, no pruning, no argument gymnastics. Wins for tiny `n` (<= 20) where the simplicity beats the backtracking version.
- Cost: enumerates 2^n masks to find C(n, k) results -- fine for small n, disastrous for n around 30.

## Alternative -- combinations with repetition

- Same code but recurse with `i` instead of `i + 1`: each level may pick the same value again. Produces C(n + k - 1, k) combinations.
- Models "k items drawn from n types with replacement" -- multiset selections, dice sum outcomes.

## Usage

- Lottery and committee selection: enumerate all ways to pick k from n.
- n-choose-k rollouts in probabilistic algorithms, uniform sampling of fixed-size subsets.
- Combinatorial search: try every size-k feature set, every k-edge subgraph, every k-element committee.
- Any "all ways to choose k of n" question in scheduling, sampling, and design of experiments.

## Cousins & contrasts

- **Subsets**: same start-index walk with no size cap -- record at every node, 2^n leaves.
- **Permutations**: branches over all unused items with `used[]` or in-place swap -- n! ordered leaves.
- **Combinations with repetition**: recurse with `i` not `i + 1` -- C(n + k - 1, k) leaves.
- **Pascal's triangle**: row n is C(n, 0), C(n, 1), ..., C(n, n) -- the same numbers, computed additively.
