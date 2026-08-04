# Permutations -- all n! orderings: any unused value can take the next slot, swapped in and swapped back in O(1)

## Core idea

- Invariant: at depth k the slots before k are fixed; the tail `a[k..]` holds exactly the unused values, so the candidate pool lives in the array itself.
- Mechanism: swap a tail value into slot k, recurse, swap it back; each restore leaves the tail intact for the next try, so every ordering is built exactly once.

## Build up

1. **Swap any candidate in**

```
swap(a[k], a[i]);      // a[i] now owns slot k
```

2. **Recurse on the next slot**

```
backtrack(a, k + 1, out);
```

3. **Swap it back**

```
swap(a[k], a[i]);      // tail a[k..] intact again
```

4. **Every slot fixed: done**

```
if (k == (int)a.size()) out.push_back(a);
```

## Diagram

```
a = [1 2 3]
level 0   [1 2 3]              swap a[0] with each tail value
        /    |    \
level 1 [1 2 3] [2 1 3] [3 2 1]
        |     |     |
level 2 123 132 213 231 321 312    6 leaves = 3!
```

## Approach -- backtracking with in-place swap

```cpp
using namespace std;

static void backtrack(vector<int>& a, int k, vector<vector<int>>& out) {
    if (k == (int)a.size()) {            // step 4: all slots fixed
        out.push_back(a);
        return;
    }
    for (int i = k; i < (int)a.size(); ++i) {
        swap(a[k], a[i]);                // step 1: commit candidate
        backtrack(a, k + 1, out);        // step 2: fill next slot
        swap(a[k], a[i]);                // step 3: restore tail
    }
}

vector<vector<int>> permutations(const vector<int>& a) {
    vector<vector<int>> out;
    vector<int> nums = a;
    backtrack(nums, 0, out);
    return out;
}
```

- The undo swap is mandatory: without it the tail `a[k..]` would not look identical before each loop iteration.
- The caller's array is copied into `nums` so it is never mutated.

### Trace

- On `[1,2,3]`: fix 1, permute `[2,3]` -> 123, 132; swap in 2 -> 213, 231; swap in 3 -> 321, 312.

## Complexity

- Time: O(n * n!) to write the n! results. Space: O(n) depth plus the results.

## Alternative -- backtracking with a used flag

- Build `curr` with `vector<bool> used` instead of swapping; costs an array but extends to duplicates: sort first, then skip `a[i] == a[i-1]` when the previous equal value is unused.

## Use when

- Enumerate every ordering of a small set: TSP tours (n <= 10), anagram lists, schedule orders.
- Reach for this when a problem says "all arrangements" or "permute" and n! fits in memory.

## Cousins

- **Combinations**: a start-index walk, order ignored: C(n,k) leaves.
- **Subsets**: include/skip choices: 2^n leaves.
- **Duplicates**: same skeleton plus the sorted skip guard.
- **next_permutation**: same output in lexicographic order, no recursion, needs sorted input.
