# Invert Binary Tree

Swap every left and right child at every node -- mirrors the tree horizontally. Any traversal order works because the swap at each node is independent of its children.

## Intuition

- Swap the children, then recurse into each child to swap their children -- this is the whole algorithm, no additional logic needed.
- The operation is its own inverse: inverting twice restores the original tree.
- Any traversal order works (pre-order is shown below) since swapping a node's children doesn't depend on child state. Post-order and BFS are equally valid.

## Approach 1 -- recursive (pre-order)

```cpp
using namespace std;

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

## Approach 2 -- iterative BFS

```cpp
queue<Node*> q;
if (root) q.push(root);
while (!q.empty()) {
    Node* n = q.front(); q.pop();
    swap(n->left, n->right);
    if (n->left)  q.push(n->left);
    if (n->right) q.push(n->right);
}
```

- Same O(n) time, O(w) space where w is the max width.
- Avoids recursion depth issues on degenerate trees; level-by-level processing matches the "mirror around the vertical axis" mental picture.

## Alternative -- post-order

- Recurse into both children first, then swap. Identical result; useful when you also want to compute something bottom-up alongside the swap.
- Choosing the order is purely about what other operations you're combining with the invert.

## Complexity

- Time: O(n) -- every node is visited and swapped once.
- Space: O(h) for recursion, O(w) for the BFS variant.

## Usage

- Flipping images or UI components mirrored horizontally.
- Tree transformations in graphics and game engines: character rigs, animation hierarchies, scene graphs.
- Testing tree operations: invert a tree and compare with a known mirror.
- Any "mirror this structure around its center" operation.

## Cousins & contrasts

- **Compare binary trees**: walks two trees in lockstep without mutating. See compare_binary_trees.
- **Symmetric tree**: checks whether a tree is a mirror of itself -- uses a cross-shaped compare (left.left vs right.right, left.right vs right.left).
- **Rotate (BST / AVL)**: a single, local inversion -- swapping a node with one of its children while preserving the in-order sequence.
- **BFS / DFS with child swap**: every traversal that visits both children can be re-purposed into an invert.
