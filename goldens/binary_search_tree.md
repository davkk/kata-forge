# Binary Search Tree -- left < node < right: each comparison halves the tree

## Core idea

- Invariant: every value in a node's left subtree is smaller and every value in its right subtree is larger -- the sorted order embedded in pointers.
- Mechanism: search, insert, and delete are the same root-to-leaf walk; each comparison discards one whole subtree, so ops are O(h) with h = log n only while balanced.

## Build up

1. **The ordering rule**

```
left subtree < node->val < right subtree      // holds for every node
```

2. **Find: compare and descend**

```
while (cur) {
    if (val == cur->val) return val;
    cur = val < cur->val ? cur->left : cur->right;
}
return nullopt;
```

3. **Insert: recurse to a leaf**

```
if (!n) return new Node{v, nullptr, nullptr};
if (v < n->val) n->left = insert(n->left, v);
else            n->right = insert(n->right, v);
return n;                       // re-link on the way up
```

4. **Remove: splice or copy successor**

```
if (!n->left) return n->right;              // 0 or 1 child: splice
Node* s = n->right;
while (s->left) s = s->left;                // 2 children: in-order successor
n->val = s->val;
n->right = remove(n->right, s->val);        // delete the now-easy successor
```

## Diagram

```
insert 50, 30, 80, 20, 40, 70, 90           find(40):
        50                                   40 < 50 -> left
       /  \                                  40 > 30 -> right
      30   80                                40 == 40 -> found, 3 comparisons
     / \   / \
    20 40 70 90
```

## Approach -- recursive operations

```cpp
using namespace std;

class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree() : root(nullptr) {}

    void insert(int val) {
        root = insert(root, val);           // step 3: re-link
    }

    void remove(int val) {
        root = remove(root, val);
    }

    optional<int> find(int val) {
        Node* cur = root;
        while (cur) {                       // step 2: the walk
            if (val == cur->val) return val;
            cur = val < cur->val ? cur->left : cur->right;
        }
        return nullopt;
    }

    int size() {
        return size(root);
    }

    Node* insert(Node* n, int v) {
        if (!n) return new Node{v, nullptr, nullptr}; // step 3: hang a leaf
        if (v < n->val) n->left = insert(n->left, v); // step 1: ordering rule
        else            n->right = insert(n->right, v);
        return n;                           // step 3: re-link on the way up
    }

    Node* remove(Node* n, int v) {
        if (!n) return nullptr;
        if      (v < n->val) n->left = remove(n->left, v);
        else if (v > n->val) n->right = remove(n->right, v);
        else {
            if (!n->left)  { Node* r = n->right; delete n; return r; } // step 4
            if (!n->right) { Node* l = n->left;  delete n; return l; }
            Node* s = n->right;             // step 4: successor
            while (s->left) s = s->left;
            n->val = s->val;
            n->right = remove(n->right, s->val);
        }
        return n;
    }

    int size(Node* n) {
        return n ? 1 + size(n->left) + size(n->right) : 0;
    }
};
```

- Step 1 is the rule the branches encode; the walk (2) is the skeleton that insert (3) and remove (4) reuse with a mutation at the end.
- `find` returns the value (or `nullopt`), not a bool -- matches the catalog contract.

### Trace

- insert 50,30,80,20,40,70,90 builds the balanced tree above; find(20): left, left, hit -> 20; find(100) -> nullopt.
- remove(20) splices an empty child; remove(80) copies successor 90 into it, then deletes the 90 leaf.

## Complexity

- Time: O(h) per op (h = O(log n) balanced, O(n) if sorted input degenerates it). Space: O(n).

## Alternative -- self-balancing (AVL, red-black)

- Same invariant plus rotations -- local rewires that preserve the in-order sequence -- keeping h = O(log n) under any insertion order; what `std::map` and `std::set` are.

## Use when

- Reach for this when you need ordered keys with fast insert/delete: sorted iteration, range queries, predecessor/successor.
- A dynamic sorted array -- a sorted vector's O(n) inserts become O(log n) walks.

## Cousins

- **Hash table**: O(1) average, but no order, no ranges, no successor queries.
- **Binary search**: the same halving on array indices; a BST is the mutable version.
- **Heap**: orders parent vs children, not left vs right -- only the extreme is reachable.
- **B-tree**: the disk generalization -- what database indices actually use.
