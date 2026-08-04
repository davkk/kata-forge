# Compare Binary Trees -- walk both in lockstep: every aligned pair must agree

## Core idea

- Invariant: equality holds iff roots agree and, recursively, both child pairs agree -- the structural definition of "same tree" projected onto one pair at a time.
- Mechanism: two pointers descend identically (both left, both right); a node facing a null is a shape gap that kills the compare.

## Build up

1. **Two pointers, one per tree**

```
bool compare(Node* a, Node* b)
```

2. **Both null -> equal**

```
if (!a && !b) return true;
```

3. **One null -> shape gap**

```
if (!a || !b) return false;
```

4. **Values must match**

```
if (a->val != b->val) return false;
```

5. **Recurse into both child pairs**

```
return compare(a->left, b->left) &&
       compare(a->right, b->right);
```

## Diagram

```
  A:    a          B:    a
       / \              / \
      b   c            b   c
     /                / \
    d                d   e

  (a,a) ok
   (b,b) ok       (c,c) ok
   (d,d) ok       (null,null) ok
   (null,e) FALSE -- one null, one node
```

## Approach -- recursive lockstep

```cpp
using namespace std;

bool compare(Node* a, Node* b) {
    if (!a && !b) return true;            // step 2
    if (!a || !b) return false;           // step 3
    if (a->val != b->val) return false;   // step 4
    return compare(a->left, b->left) &&   // step 5
           compare(a->right, b->right);
}
```

- The clause order matters: `!a || !b` must come after `!a && !b`, or a null would be dereferenced.
- `&&` short-circuits: a failed left pair skips the right subtrees entirely.

### Trace

- Identical trees recurse lockstep all the way down, every pair true, then back up -- true.
- Extra node 21 under 29 in `b` only: pair becomes (null, 21) -> `!a` true, `!b` false -> false; b's right side never runs.

## Complexity

- Time: O(n) -- each aligned pair touched once. Space: O(h) recursion (O(n) degenerate).

## Alternative -- serialize with null markers

- Flatten both trees pre-order, writing explicit nulls, then string-compare: O(n) time but O(n) scratch space; worth it only if you must store the shape anyway.

## Use when

- Reach for this when asked "same tree" or "deeply equal" -- DOM, JSON, AST equality.
- Symmetric tree: compare(a->left, b->right) && compare(a->right, b->left) on one root.
- Subtree of another tree: probe compare at every node of the larger tree.

## Cousins

- **invert_tree**: the destructive twin -- compare reads, invert mutates.
- **Tree isomorphism**: shape only, drop the value check.
- **DFS**: one pointer over one tree; compare pairs two pointers at the same slot.
