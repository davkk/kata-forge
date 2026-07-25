# Invert Binary Tree

Swap every left and right child at every node — mirrors the tree horizontally. Any traversal order works because the swap at each node is independent of its children.

## Intuition

- Swap the children, then recurse into each child to swap their children — this is the whole algorithm, no additional logic needed.
- The operation is its own inverse: inverting twice restores the original tree.
- Any traversal order works (pre-order is shown below) since swapping a node's children doesn't depend on child state. Post-order and BFS are equally valid.

## Recursive (pre-order)

```cpp
using namespace std;

struct Node { int value; Node* left; Node* right; };

Node* invert_tree(Node* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invert_tree(root->left);
    invert_tree(root->right);
    return root;
}
```

- `std::swap` eliminates the temp variable; the entire body is swap + two recursions.
- Mutates the tree in place; returns the root for caller convenience.

## Where it shows up

- Flipping images or UI components mirrored horizontally.
- The "homebrew problem" that went viral after Max Howell was rejected by Google for not solving it — now a classic interview warm-up.
- Testing tree operations: invert a tree and compare with a known mirror.

## Cousins & contrasts

- **Compare binary trees**: walks two trees in lockstep without mutating. See compare_binary_trees.
- **Symmetric tree**: checks whether a tree is a mirror of itself — uses a cross-shaped compare (left.left vs right.right, left.right vs right.left).
