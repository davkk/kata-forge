# Binary Search Tree

A binary tree with the ordering invariant: for every node, **every** value in its left subtree is smaller and **every** value in its right subtree is larger. Search, insert, and delete are all O(h).

## Intuition

- The invariant applies to whole **subtrees, not just direct children** -- checking only `left->value < node->value < right->value` is the classic validate-BST bug.
- Search and insert are the same walk: each comparison discards half the remaining tree, exactly like binary search on a sorted array.
- All operations are O(h); h = log n only if the tree stays balanced. Sorted input builds a linked list and everything degrades to O(n) -- the shape depends entirely on insertion order, which is why self-balancing variants exist.
- In-order traversal yields the values sorted -- many BST problems reduce to that fact.

## Approach -- recursive operations

```cpp
using namespace std;

struct BinarySearchTree {
    Node* root = nullptr;

    void insert(int val) {
        root = insert(root, val);
    }

    void erase(int val) {
        root = erase(root, val);
    }

    optional<int> find(int val) {
        Node* cur = root;
        while (cur) {
            if (val == cur->val) return val;
            cur = val < cur->val ? cur->left : cur->right;
        }
        return nullopt;
    }

    int length() {
        return length(root);
    }

    Node* insert(Node* n, int v) {
        if (!n) return new Node{v, nullptr, nullptr};
        if (v < n->val) n->left = insert(n->left, v);
        else            n->right = insert(n->right, v);
        return n;
    }

    Node* erase(Node* n, int v) {
        if (!n) return nullptr;
        if      (v < n->val) n->left = erase(n->left, v);
        else if (v > n->val) n->right = erase(n->right, v);
        else {
            if (!n->left)  { Node* r = n->right; delete n; return r; }
            if (!n->right) { Node* l = n->left;  delete n; return l; }
            Node* s = n->right;
            while (s->left) s = s->left;
            n->val = s->val;
            n->right = erase(n->right, s->val);
        }
        return n;
    }

    int length(Node* n) {
        return n ? 1 + length(n->left) + length(n->right) : 0;
    }
};
```

- Public `insert`/`erase` delegate to overloaded helpers that return the new subtree root -- the caller re-links it, eliminating all parent-pointer bookkeeping.
- Delete has exactly three cases: **leaf or one child** -- splice the child (or null) up; **two children** -- copy the in-order successor's value into the node, then delete the successor, which always has at most one child.
- `find` returns `nullopt` on miss instead of a null pointer.
- `length` does a full traversal each call; for hot-path usage, cache it as a member counter bumped on insert/erase. Duplicates go right; reject `v == n->val` for set semantics.

## Alternative -- self-balancing BSTs (AVL, red-black)

- Same invariant plus a balance rule maintained by **rotations** -- local pointer rewires that preserve the in-order sequence.
- Guarantees h = O(log n) regardless of insertion order; this is what `std::map` and `std::set` are.
- Write the plain BST first; reach for balancing when input order is adversarial or sorted.

## Alternative -- order-statistics tree (kth element)

- Augment each node with the size of its left subtree, so kth-smallest (and rank queries) drop out in O(log n).
- Library of useful extra operations (select, rank, count of keys in a range) on top of the same BST skeleton; slightly more code per operation.

## Complexity

- Time: O(h) per operation (h = O(log n) when balanced, O(n) when degenerate). Space: O(n).

## Usage

- Ordered dictionaries: `std::map`/`std::set`, database indices (B-trees are the disk-friendly generalization), autocomplete-style ordered scans.
- Spell checkers and file-system directories: ordered storage with fast lookup plus sorted enumeration.
- Anywhere a sorted array would do, but you also need fast insert/delete -- the dynamic version of a sorted array.

## Cousins & contrasts

- **Hash table**: O(1) average lookup but unordered -- no sorted iteration, no range queries. Pay O(log n) when order matters.
- **Binary search on a sorted array**: the same halving logic on indices; a balanced BST is a *dynamic* sorted array.
- **DFS on a BST**: the search half of this structure in isolation. See dfs_on_bst.
- **Heap**: also a shaped binary tree, but it orders parent vs children (not left vs right) -- min/max only, no sorted walk.
