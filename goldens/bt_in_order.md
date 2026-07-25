# In-Order Traversal

Depth-first traversal that visits **left, then root, then right**. On a binary search tree this yields values in **sorted ascending order** — that one fact drives a whole family of BST problems.

## Intuition

- All three DFS orders walk the same edges in the same sequence; the only difference is *when* you visit the node relative to its children. In-order visits between the two recursive calls.
- Why sorted on a BST: everything in the left subtree is smaller than the node, everything in the right is larger — so "left, node, right" reads exactly as "smaller, equal, larger".
- Sorted output means order questions (kth smallest, validate BST, predecessor) are answered by a single walk, with no sort pass.
- Time O(n). Space O(h) recursion stack — O(n) for a degenerate tree.

## Recursive

```cpp
using namespace std;

struct Node { int value; Node* left; Node* right; };

void walk(Node* curr, vector<int>& out) {
    if (!curr) return;
    walk(curr->left, out);        // left
    out.push_back(curr->value);   // visit: root
    walk(curr->right, out);       // right
}

vector<int> in_order(Node* root) {
    vector<int> out;
    walk(root, out);
    return out;
}
```

- Identical skeleton to pre-order — only the `push_back` line moved.
- Early-exit variant: carry a counter and stop at the kth visited node instead of building the whole list.

## Alternative: iterative & O(1)-space variants

- **Iterative with a stack**: push the whole left chain, then pop, visit, and step into the right child; repeat. The visit happens at pop time, after the left side is exhausted.
- **Morris traversal**: O(1) extra space — temporarily thread each left subtree's rightmost node back to the current node, then unthread on the second pass. No stack at all, at the cost of mutating the tree mid-walk.

## Where it shows up

- Dumping a BST in sorted order — e.g. an in-memory database index scan.
- **Validate BST**: in-order output must be strictly increasing; check adjacent pairs during the walk.
- **Kth smallest in a BST**: count visits in-order, stop at k.
- Flattening a BST to a sorted array (the inverse build picks array midpoints as roots).

## Cousins & contrasts

- **Pre-order / post-order**: same walk, different visit timing — pre-order preserves structure, post-order aggregates bottom-up. See bt_pre_order, bt_post_order.
- **BFS**: level-by-level with a queue; gives you levels, not sortedness. See bt_bfs.
- **Binary search on a sorted array**: the array is the flattened in-order output; a balanced BST is that array with links instead of indices.
