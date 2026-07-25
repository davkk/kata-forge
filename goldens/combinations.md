# Combinations

All C(n,k) k-element subsets of `{1..n}` — subsets of size exactly k, order irrelevant.

## Intuition

- Shared skeleton: **choose → explore → unchoose** down a decision tree. The branching rule: pick the **next item from a start index onward** (like subsets), but stop once `curr` reaches size k.
- The start index guarantees increasing order inside every combination, so `{1,3}` appears once and `{3,1}` never exists.
- Leaves sit at depth k → exactly **C(n,k) = n! / (k!(n−k)!)** leaves.
- **Prune**: if `curr.size() + (n − i + 1) < k`, remaining items can't fill k — break the loop. This keeps work at Θ(C(n,k)).
- Complexity: O(k·C(n,k)) time (output copies), O(k) recursion depth.

## Backtracking with start index + size cap

```cpp
using namespace std;

static void backtrack(int n, int k, int start,
                      vector<int>& curr, vector<vector<int>>& out) {
    if ((int)curr.size() == k) {
        out.push_back(curr);
        return;
    }
    for (int i = start; i <= n; ++i) {
        if ((int)curr.size() + (n - i + 1) < k) break;  // prune
        curr.push_back(i);                     // choose
        backtrack(n, k, i + 1, curr, out);     // explore
        curr.pop_back();                       // unchoose
    }
}

vector<vector<int>> combinations(int n, int k) {
    vector<vector<int>> out;
    vector<int> curr;
    backtrack(n, k, 1, curr, out);
    return out;
}
```

- `k == 0` yields one empty combination — falls out of the base case naturally.
- Pass `i + 1`, not `start + 1`; loop bound is `<= n` since values are `1..n`.
- The prune is a `break`, not `continue`: as `i` grows, the remaining count only shrinks.

## Where it shows up

- Lottery/committee selection: enumerate all ways to pick k from n.
- n-choose-k rollouts in probabilistic algorithms (e.g., uniform sampling of fixed-size subsets).
- Combinatorial search: try every size-k feature set, every k-edge subgraph.

## Cousins & contrasts

- **Subsets**: same start-index walk with no size cap — record at every node, 2ⁿ leaves.
- **Permutations**: branches over all unused items with used[] or in-place swap — n! ordered leaves.
- **Combinations with repetition**: same code but recurse with `i` (not `i + 1`) — C(n+k−1, k) leaves.
