# Compare Binary Trees

Structural and value equality: both trees have the same shape and same values at every corresponding node. This is the template pattern for "same tree", "symmetric tree", and "subtree of another tree".

## Intuition

- Walk both trees in lockstep: compare the current pair of nodes, then descend into left children together and right children together.
- Three base cases cover everything: both null -> true, exactly one null -> false, value mismatch -> false.
- Any traversal order works as long as both sides are visited identically; pre-order lockstep is the most natural.
- O(n) time, O(h) recursion depth (worst case O(n) for a degenerate tree).

## Approach -- recursive

```cpp
using namespace std;

bool compare(Node* a, Node* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    if (a->val != b->val) return false;
    return compare(a->left, b->left) && compare(a->right, b->right);
}
```

- `!a || !b` catches "exactly one is null" in a single line.
- Short-circuit `&&` skips the right subtree when the left already differs -- no wasted work.

## Alternative -- serialize and compare

- Flatten both trees to strings (pre-order with null markers), then do a string comparison.
- Same O(n) time but needs O(n) extra space for the strings. The recursive lockstep is simpler and standard.
- Useful when you also need to store / transmit the tree shape (caches, network messages).

## Alternative -- iterative (explicit stack)

- Push `(a, b)` pairs onto a stack; for each pair, check the same three base cases and push `(a->left, b->left)`, `(a->right, b->right)`.
- Same O(n) time, O(h) space (in practice often less than recursion because the stack is shared).
- Reach for it on degenerate trees where recursion depth would overflow.

## Complexity

- Time: O(n) where n is the number of nodes in the larger tree.
- Space: O(h) recursion depth, O(n) in the worst case for a degenerate tree.

## Usage

- Testing structural equality of DOM trees or JSON objects.
- **Symmetric tree**: compare the left and right subtrees of a single root with swapped arguments -- `compare(a->left, b->right) && compare(a->right, b->left)`.
- **Subtree of another tree**: for each node in the larger tree, check if `compare` returns true with the smaller tree.
- Diff tools for tree-shaped data (XML/JSON diff, AST comparison in compilers).

## Cousins & contrasts

- **Invert tree**: the destructive mirror; compare is read-only. See invert_tree.
- **DFS traversal**: visits one tree; compare walks two trees simultaneously, pairing nodes.
- **Same-tree variant**: identical code; the symmetric-tree variant swaps the recursion arguments.
- **Tree isomorphism**: shape-only equality (values may differ) -- drop the value check, keep the structural one.
