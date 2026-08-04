# Subsets -- all 2^n choices: record at every recursion node while a start index sweeps increasing picks

## Core idea

- Invariant: every recursion node is itself a valid subset, and picks only move forward, so each subset is recorded once in increasing index order.
- Mechanism: push, recurse, pop bracket each choice; the loop's next index is the "skip" branch, so take/skip is covered without an explicit skip call.

## Build up

1. **Record every node**

```
out.push_back(curr);    // every prefix is a subset
```

2. **Take the next element**

```
curr.push_back(a[i]);
backtrack(a, i + 1, curr, out);
```

3. **Put it back**

```
curr.pop_back();
```

4. **Next i skips a[i]**

```
for (int i = start; i < (int)a.size(); ++i) ...
```

## Diagram

```
a = [1 2 3], record at EVERY node
                   []
            take /     \ skip
              [1]        []
            /    \      /    \
         [12]    [1]  [2]    []
        /  \    /  \  /  \  /  \
     123  12  13  1  23  2  3  []    8 nodes at depth 3 = 2^3
```

## Approach -- backtracking with start index

```cpp
using namespace std;

static void backtrack(const vector<int>& a, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    out.push_back(curr);             // step 1: record every node
    for (int i = start; i < (int)a.size(); ++i) {  // step 4: skip = next i
        curr.push_back(a[i]);        // step 2: take a[i]
        backtrack(a, i + 1, curr, out);
        curr.pop_back();             // step 3: put it back
    }
}

vector<vector<int>> subsets(const vector<int>& a) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(a, 0, curr, out);
    return out;
}
```

- The record sits before the loop, so the empty subset is written first on every call.
- Recurse with `i + 1` to keep the start index growing: uniqueness comes from never rewinding.

### Trace

- On `[1,2,3]`: `[]`, take 1 -> `[1]`, take 2 -> `[12]`, take 3 -> `[123]`, unwind, skip 2, take 3 -> `[13]`, unwind, take 2 -> `[2]`, `[23]`, then `[3]`. All 8.

## Complexity

- Time: O(n * 2^n), the output dominates. Space: O(n) depth plus the results.

## Alternative -- bitmask enumeration

- Each mask from 0 to 2^n - 1 is a subset: bit j set means include a[j]. No recursion; best under n <= 20.

## Use when

- Enumerate every inclusion choice over a small set: feature flags, exhaustive tests, subset-sum, meet-in-the-middle halves.
- Reach for this when a problem asks for "all subsets" and 2^n fits in memory.

## Cousins

- **Permutations**: branch over all unused items: n! leaves.
- **Combinations**: the same walk, stopping at depth k: C(n,k) leaves.
- **Subsets with duplicates**: sort first, skip `a[i] == a[i-1]` at the same level.
- **Iterative build**: start with `[[]]`, append each element to every existing subset.
