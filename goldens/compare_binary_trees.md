# Compare Binary Trees

Structural and value equality: both trees have the same shape and same values at every corresponding node. This is the template pattern for "same tree", "symmetric tree", and "subtree of another tree".

## Intuition

- Walk both trees in lockstep: compare the current pair of nodes, then descend into left children together and right children together.
- Three base cases cover everything: both null → true, exactly one null → false, value mismatch → false.
- Any traversal order works as long as both sides are visited identically; pre-order lockstep is the most natural.

## Recursive

```cpp
using namespace std;

struct Node { int val; Node* left; Node* right; };

bool compare(Node* a, Node* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    if (a->val != b->val) return false;
    return compare(a->left, b->left) && compare(a->right, b->right);
}
```

- `!a || !b` catches "exactly one is null" in a single line.
- Short-circuit `&&` skips the right subtree when the left already differs — no wasted work.

## Alternative: serialize and compare

- Flatten both trees to strings (pre-order with null markers), then do a string comparison.
- Same O(n) time but needs O(n) extra space for the strings. The recursive lockstep is simpler and standard.

## Where it shows up

- Testing structural equality of DOM trees or JSON objects.
- **Symmetric tree**: compare the left and right subtrees of a single root with swapped arguments — `compare(a->left, b->right) && compare(a->right, b->left)`.
- **Subtree of another tree**: for each node in the larger tree, check if `compare` returns true with the smaller tree.

## Cousins & contrasts

- **Invert tree**: the destructive mirror; compare is read-only. See invert_tree.
- **DFS traversal**: visits one tree; compare walks two trees simultaneously, pairing nodes.
