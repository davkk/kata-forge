# Pre-Order Traversal

Depth-first traversal that visits **root, then left, then right** — the node is processed before its children, so the output preserves the tree's nesting.

## Intuition

- All three DFS orders walk the same edges in the same sequence; the only difference is *when* you visit the node relative to its children. Pre-order visits on the way down, before recursing.
- Because a parent is emitted before its whole subtree, the first element is always the root and each value is followed by its entire left subtree, then its entire right — which is why pre-order is the serialization order.
- Time O(n): every node is visited exactly once. Space O(h) for the recursion stack — O(n) worst case for a degenerate, linked-list-shaped tree.

## Recursive

```cpp
using namespace std;

struct Node { int value; Node* left; Node* right; };

void walk(Node* curr, vector<int>& out) {
    if (!curr) return;
    out.push_back(curr->value);   // visit: root
    walk(curr->left, out);        // then left
    walk(curr->right, out);       // then right
}

vector<int> pre_order(Node* root) {
    vector<int> out;
    walk(root, out);
    return out;
}
```

- Base case: a null child contributes nothing — just return.
- The three lines are the entire algorithm; moving the `push_back` gives you in-order and post-order.

## Alternative: iterative with explicit stack

- Replace the call stack with your own: push the right child first, then the left, so the left pops first (the stack is LIFO).
- Same O(n) / O(h), but no recursion-depth limit. The classic bug is pushing left first and getting a mirrored order.

```cpp
using namespace std;

vector<int> pre_order(Node* root) {
    vector<int> out;
    if (!root) return out;
    stack<Node*> st;
    st.push(root);
    while (!st.empty()) {
        Node* curr = st.top(); st.pop();
        out.push_back(curr->value);
        if (curr->right) st.push(curr->right);  // right goes in first…
        if (curr->left)  st.push(curr->left);   // …so left pops first
    }
    return out;
}
```

## Where it shows up

- Deep-copying a tree: create the node, then recurse into its children — pre-order is the natural copy order.
- Serializing trees (with null markers) so they can be rebuilt; prefix (Polish) notation of expression trees.
- Any "parent before children" pattern: propagating values downward, enumerating root-to-leaf paths.

## Cousins & contrasts

- **In-order** (left, root, right): same walk, visit in the middle — yields sorted order on a BST. See bt_in_order.
- **Post-order** (left, right, root): visit on the way up — children before parent, for deletion and bottom-up aggregation. See bt_post_order.
- **BFS**: level-by-level with a queue instead of diving with a stack. See bt_bfs.
