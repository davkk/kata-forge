# DFS on a BST

Searching a binary search tree by exploiting its **ordering invariant** (left < node < right): at every node one comparison tells you which subtree can possibly hold the target, so you walk **a single root-to-leaf path** -- O(h), not O(n).

## Intuition

- This is binary search as pointer-chasing: each node is a `mid`, and you discard half the remaining tree per step.
- Contrast with DFS on an unordered tree: no ordering means no pruning -- you must visit every node, O(n).
- O(h) time: h ~= log n for a balanced tree, but n for a degenerate one -- the search is only as good as the tree's shape.
- The iterative form needs no stack at all: you walk one path, so a plain while loop gives O(1) space.

## Approach 1 -- recursive

```cpp
using namespace std;

struct Node { int val; Node* left; Node* right; };

bool dfs(Node* curr, int needle) {
    if (!curr) return false;
    if (curr->val == needle) return true;
    if (needle < curr->val) return dfs(curr->left, needle);
    return dfs(curr->right, needle);
}
```

- Never recurse into both sides -- the moment you do, you've thrown away the BST advantage.
- A missing value walks off a leaf and hits the null base case: "not found" costs the same O(h).

## Approach 2 -- iterative (O(1) space)

```cpp
bool dfs(Node* curr, int needle) {
    while (curr) {
        if (curr->val == needle) return true;
        curr = needle < curr->val ? curr->left : curr->right;
    }
    return false;
}
```

- The recursion is tail recursion -- it collapses into a trivial while loop with O(1) space.
- Same O(h) time, no risk of stack overflow on degenerate trees.

## Alternative -- lower bound / upper bound (range queries)

- Return the first node `>= needle` (lower bound) or the first `> needle` (upper bound) by walking the same path and keeping the best candidate seen.
- "Closest value" to a target: track the best-so-far while walking, update when `|node - needle|` improves.
- "Floor / ceiling" are the same idea with `<` / `>` comparisons.

## Complexity

- Time: O(h) -- O(log n) for a balanced tree, O(n) for a degenerate one.
- Space: O(1) iterative, O(h) recursive (worst case O(n)).

## Usage

- Lookup, insert, and delete in a BST all begin with this exact walk. See binary_search_tree.
- Range queries ("all values between X and Y"): prune subtrees that can't overlap the range -- partial pruning, still better than a full scan.
- Closest value / floor / ceiling in a BST: track the best candidate seen while descending.
- Any "is this value here, and if not, where would it go" question on a sorted set.

## Cousins & contrasts

- **Full tree DFS** (pre/in/post-order): explores everything, O(n); the right tool when the tree has no ordering to exploit.
- **BFS on a tree**: still O(n) and pointless for search in a BST -- ordering beats breadth. See bt_bfs.
- **Binary search on a sorted array**: the same halving logic on indices instead of pointers; the array is a flattened balanced BST.
- **DFS on a graph**: needs a `seen` set to avoid revisiting; BSTs never revisit, so the check is implicit.
