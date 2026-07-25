# Permutations

All n! orderings of a list of **distinct** numbers. Backtracking where the branching rule is "any unused item may go next".

## Intuition

- Shared skeleton: **choose → explore → unchoose** down a decision tree. Position `k` is the level; try every still-unused item at that position — a tree of degree n, n−1, … → **n! leaves**.
- The swap variant tracks "unused" in place: elements left of `k` are fixed, elements at `k..n−1` are the candidate pool. Swap a candidate into `k`, recurse, swap back.
- Every permutation is generated exactly once because each path fixes a distinct position with a distinct value.
- Complexity: O(n·n!) time (output dominates), O(n) extra space (mutate in place).

## Backtracking with in-place swap

```cpp
using namespace std;

static void backtrack(vector<int>& a, int k, vector<vector<int>>& out) {
    if (k == (int)a.size()) {
        out.push_back(a);
        return;
    }
    for (int i = k; i < (int)a.size(); ++i) {
        swap(a[k], a[i]);                // choose
        backtrack(a, k + 1, out);        // explore
        swap(a[k], a[i]);                // unchoose
    }
}

vector<vector<int>> permutations(const vector<int>& nums) {
    vector<vector<int>> out;
    vector<int> a = nums;
    backtrack(a, 0, out);
    return out;
}
```

- The undo swap is mandatory: the pool at `k..n−1` must be identical before every loop iteration.
- A `used[]` boolean + build-list works too; the swap version just saves the extra array.
- **Duplicates pitfall**: sort first, then at each level skip values equal to the one just tried (`if (i > k && a[i] == a[i-1]) continue;`) — otherwise the same ordering repeats.

## Where it shows up

- Brute-force small cases: TSP with ≤ 10 cities, assignment problems, anagram solvers.
- Scheduling and ordering search where feasibility is checked per ordering.
- Base generator for shuffle testing and permutation-based heuristics.

## Cousins & contrasts

- **Subsets**: same skeleton, each item branches include/exclude — 2ⁿ leaves.
- **Combinations**: start-index walk stopped at depth k — C(n,k) leaves.
- **Heap's algorithm**: generates all n! permutations with exactly one swap per permutation — minimal swaps; trickier to recall.
- **`std::next_permutation`**: lexicographic successor; `do { ... } while (next_permutation(a.begin(), a.end()));` on a sorted array is the interview-safe one-liner.
