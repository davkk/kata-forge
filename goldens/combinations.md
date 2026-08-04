# Combinations -- pick k of n, order ignored: a start index that never moves back keeps exactly C(n,k) sequences

## Core idea

- Invariant: every pick is larger than the last, so each combination has one canonical increasing form and `{1,3}` can never be rebuilt as `{3,1}`.
- Mechanism: pick, recurse past the pick, undo the pick; depth k is a leaf, so the walk visits each k-subset exactly once.

## Build up

1. **Pick an element, recurse**

```
curr.push_back(i);
backtrack(n, k, i + 1, curr, out);
```

2. **Restore the choice**

```
curr.pop_back();
```

3. **Prune with start index**

```
for (int i = start; i <= n; ++i)   // i only grows: no repeats
```

## Diagram

```
n = 4, k = 2                     []
               /       |        |       \
             1        2        3        4 [dead]
           / | \     / \       |        only 1 left,
          2  3  4   3   4      4        needs 2: pruned
        [12][13][14][23][24]  [34]
```

## Approach -- backtracking with start index

```cpp
using namespace std;

static void backtrack(int n, int k, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    if ((int)curr.size() == k) {         // depth k is a leaf: save
        out.push_back(curr);
        return;
    }
    for (int i = start; i <= n; ++i) {   // step 3: start only grows
        if ((int)curr.size() + (n - i + 1) < k) break;
        curr.push_back(i);               // step 1: pick
        backtrack(n, k, i + 1, curr, out);
        curr.pop_back();                 // step 2: undo
    }
}

vector<vector<int>> combinations(int n, int k) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(n, k, 1, curr, out);
    return out;
}
```

- The prune is a `break`: as `i` grows, the values left `n - i + 1` only shrink, so a doomed branch never recovers.
- `k == 0` records `[]` at once: the empty combination falls out of the base case with no special handling.

### Trace

- `combinations(4, 2)`: pick 1 then 2, 3, 4; pick 2 then 3, 4; pick 3 then 4; the branch at 4 is pruned. Six leaves = C(4,2).

## Complexity

- Time: O(k * C(n,k)) to write the output; internal nodes stay O(C(n,k)). Space: O(k) depth plus the results.

## Alternative -- bitmask enumeration

- Try all 2^n masks and keep those with popcount k: no recursion, but scans 2^n masks, so only for n <= 20.

## Use when

- Enumerate fixed-size subgroups where composition has no order: committees, lottery picks, feature sets.
- Reach for this when a problem asks "choose k of n" and needs every unordered selection listed.

## Cousins

- **Subsets**: the same start-index walk, but record at every node: 2^n results.
- **Permutations**: branch over all unused items: n! ordered results.
- **Combinations with repetition**: recurse with `i`, not `i + 1`: C(n+k-1, k) multisets.
