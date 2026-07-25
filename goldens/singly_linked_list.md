# Singly Linked List

Nodes chained by a single `next` pointer: O(1) prepend, O(n) access by index. The whole game is **pointer rewiring without losing the rest of the list**.

## Intuition

- A list is fully described by its `head`; empty means `head == nullptr`.
- Walking is the fundamental op: `get`/`insertAt`/`removeAt` all reduce to "walk to the node at (or just before) the index, then read or splice".
- Every splice needs the node *before* the target — hence the walk-to-`prev` pattern, or a dummy head node so index 0 stops being a special case.
- O(1) prepend, O(n) append (unless you also keep a `tail`), O(n) indexed access. Space O(n) with one pointer of overhead per element.

## Walking + splicing

```cpp
using namespace std;

struct Node { int val; Node* next; };

Node* getNode(Node* head, int idx) {
    for (int i = 0; i < idx && head; ++i) head = head->next;
    return head;                         // nullptr if idx out of range
}

Node* prepend(Node* head, int x) {
    return new Node{x, head};            // new node points at old head
}

void insertAt(Node* head, int x, int idx) {   // idx >= 1
    Node* prev = getNode(head, idx - 1);
    if (!prev) return;
    prev->next = new Node{x, prev->next};
}

int removeAt(Node*& head, int idx) {
    if (idx == 0) {                      // pop front — head itself changes
        if (!head) return -1;
        Node* dead = head;
        head = head->next;
        int val = dead->val; delete dead;
        return val;
    }
    Node* prev = getNode(head, idx - 1);
    if (!prev || !prev->next) return -1; // idx out of range
    Node* dead = prev->next;
    prev->next = dead->next;             // splice out
    int val = dead->val; delete dead;
    return val;
}
```

- Functions that can replace the head take `Node*&` (or return the new head); the rest take `Node*` and assume a non-empty list.
- Link order matters: connect the new node to `prev->next` *before* overwriting `prev->next` — reverse it and everything past `prev` is gone.
- Guard every `->` with a null check: `prev->next` is null when the target is past the end.
- Dummy-head trick: allocate a fake node in front of `head` so insert/remove at index 0 uses the same "splice after prev" code as every other position.

## Alternative: in-place reversal (3 pointers)

- The single most-asked list operation: flip every arrow, return the old tail as the new head. O(n) time, O(1) space, no allocation.
- Walk with `prev` (reversed prefix), `cur` (node being flipped), `nxt` (saved remainder). Save `cur->next` *before* overwriting it — same "don't lose the rest" rule.

```cpp
Node* reverse(Node* head) {
    Node *prev = nullptr, *cur = head;
    while (cur) {
        Node* nxt = cur->next;   // save the remainder
        cur->next = prev;        // flip the arrow
        prev = cur;              // advance both
        cur = nxt;
    }
    return prev;                 // old tail = new head
}
```

## Where it shows up

- Adjacency lists in graph representations, hash-table bucket chains, undo/redo (one direction).
- Free lists inside memory allocators; kernel/embedded structures where arrays can't grow.
- Interview staples: reverse a list, detect a cycle (Floyd's tortoise/hare), merge two sorted lists, find the middle node.

## Cousins & contrasts

- **Array list**: contiguous storage with O(1) indexing — the linked list trades that away for O(1) splicing at a known position.
- **Doubly linked list**: adds a `prev` pointer so deleting a *given* node is O(1) with no walk, at one more pointer per node.
- **std::forward_list**: the STL singly linked list — its `insert_after`/`erase_after` API mirrors the "splice after prev" pattern exactly.
