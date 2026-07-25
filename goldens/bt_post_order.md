# Post-Order Traversal

Depth-first traversal that visits **left, then right, then root** — children are fully processed before their parent, which makes it the order for anything that aggregates or destroys bottom-up.

## Intuition

- All three DFS orders walk the same edges in the same sequence; the only difference is *when* you visit the node relative to its children. Post-order visits on the way back up, after both subtrees are done.
- Key invariant: when a node is visited, both its subtrees are already finished — so their results (sizes, heights, values) can be safely used, or their memory freed.
- That makes post-order the natural shape for "combine my children's answers": subtree sums, heights, directory totals, expression values.
- Time O(n). Space O(h) recursion stack — O(n) for a degenerate tree.

## Recursive

```cpp
using namespace std;

struct Node { int value; Node* left; Node* right; };

void walk(Node* curr, vector<int>& out) {
    if (!curr) return;
    walk(curr->left, out);        // left
    walk(curr->right, out);       // right
    out.push_back(curr->value);   // visit: root, last
}

vector<int> post_order(Node* root) {
    vector<int> out;
    walk(root, out);
    return out;
}
```

- Same skeleton as pre-order — the `push_back` just moved after both recursive calls.
- Freeing a tree has the same shape: recurse into both children, then `delete curr`. Never delete before recursing — you'd lose the child pointers.

## Alternative: iterative

- Two stacks: pop from the first into the second while pushing left then right — the second stack ends up in post-order. Or run iterative pre-order with left/right swapped (see bt_pre_order) and **reverse the output**.
- Both sidestep the real difficulty: an explicit stack must not visit a node before its children, which needs extra bookkeeping that the reverse trick avoids.

## Where it shows up

- Deleting/freeing a tree: children must be released before the parent.
- Directory sizes and subtree sums: a node's total is its own value plus its children's totals.
- Evaluating expression trees: operands (children) before the operator (root) — postfix / reverse-Polish notation is a post-order readout.

## Cousins & contrasts

- **Pre-order**: parent first — copying and serialization, the mirror image of deletion. See bt_pre_order.
- **In-order**: visit between the children — sorted output on a BST. See bt_in_order.
- **BFS**: level-by-level; finds the shallowest match, while post-order reaches the deepest nodes first. See bt_bfs.
