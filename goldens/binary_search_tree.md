# Binary Search Tree

A binary tree with the ordering invariant: for every node, **every** value in its left subtree is smaller and **every** value in its right subtree is larger. Search, insert, and delete are all O(h).

## Intuition

- The invariant applies to whole **subtrees, not just direct children** — checking only `left->value < node->value < right->value` is the classic validate-BST bug.
- Search and insert are the same walk: each comparison discards half the remaining tree, exactly like binary search on a sorted array.
- All operations are O(h); h = log n only if the tree stays balanced. Sorted input builds a linked list and everything degrades to O(n) — the shape depends entirely on insertion order, which is why self-balancing variants exist.
- In-order traversal yields the values sorted — many BST problems reduce to that fact.

## Operations

```cpp
using namespace std;

struct Node { int value; Node* left; Node* right; };

Node* find(Node* root, int v) {
    while (root) {
        if (v == root->value) return root;
        root = v < root->value ? root->left : root->right;
    }
    return nullptr;
}

Node* insert(Node* root, int v) {               // returns subtree root
    if (!root) return new Node{v, nullptr, nullptr};
    if (v < root->value) root->left  = insert(root->left, v);
    else                 root->right = insert(root->right, v);
    return root;
}

Node* erase(Node* root, int v) {                // returns subtree root
    if (!root) return nullptr;
    if      (v < root->value) root->left  = erase(root->left, v);
    else if (v > root->value) root->right = erase(root->right, v);
    else {
        if (!root->left)  { Node* r = root->right; delete root; return r; }
        if (!root->right) { Node* l = root->left;  delete root; return l; }
        Node* succ = root->right;               // in-order successor:
        while (succ->left) succ = succ->left;   // min of right subtree
        root->value = succ->value;
        root->right = erase(root->right, succ->value);
    }
    return root;
}
```

- `insert`/`erase` return the (possibly new) subtree root and the caller re-links it — this idiom eliminates all parent-pointer bookkeeping.
- Delete has exactly three cases: **leaf or one child** — splice the child (or null) up; **two children** — copy the in-order successor's value into the node, then delete the successor, which always has at most one child.
- Duplicates: this version inserts them to the right; reject `v == root->value` on insert for set semantics. Track `length` as a counter bumped only when a node is actually created or removed.

## Alternative: self-balancing BSTs (AVL, red-black)

- Same invariant plus a balance rule maintained by **rotations** — local pointer rewires that preserve the in-order sequence.
- Guarantees h = O(log n) regardless of insertion order; this is what `std::map` and `std::set` are.
- Write the plain BST first; reach for balancing when input order is adversarial or sorted.

## Where it shows up

- Ordered dictionaries: `std::map`/`std::set`, database indices (B-trees are the disk-friendly generalization), autocomplete-style ordered scans.
- Spell checkers and file-system directories: ordered storage with fast lookup plus sorted enumeration.
- Interview substrate: validate BST, kth smallest, lowest common ancestor, floor/ceiling.

## Cousins & contrasts

- **Hash table**: O(1) average lookup but unordered — no sorted iteration, no range queries. Pay O(log n) when order matters.
- **Binary search on a sorted array**: the same halving logic on indices; a balanced BST is a *dynamic* sorted array.
- **DFS on a BST**: the search half of this structure in isolation. See dfs_on_bst.
- **Heap**: also a shaped binary tree, but it orders parent vs children (not left vs right) — min/max only, no sorted walk.
