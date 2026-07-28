# Doubly Linked List

Each node points both ways (`prev` and `next`). The payoff over a singly linked list: **O(1) deletion given only the node** — no walk to find its predecessor.

## Intuition

- Singly linked removal needs the target's predecessor to reroute `prev->next`. With `prev` pointers, the node itself carries both neighbors: two assignments unlink it.
- Keep both `head` and `tail`: append and prepend both become O(1), no walking.
- Cost: one extra pointer per element, and every splice has twice as many assignments to get right (4 links for insert, 2 for delete).
- Indexed access is still O(n) — pointers buy splicing speed, not search speed.

## Approach — splice out a node (the reason for prev)

```cpp
using namespace std;

struct DoublyLinkedList {
    Node *head, *tail;

    void append(int x) {
        Node* n = new Node{x, tail, nullptr};
        if (tail) tail->next = n;
        else      head = n;
        tail = n;
    }

    void prepend(int x) {
        Node* n = new Node{x, nullptr, head};
        if (head) head->prev = n;
        else      tail = n;
        head = n;
    }

    optional<int> removeAt(int i) {
        Node* cur = head;
        for (int j = 0; cur && j < i; j++) cur = cur->next;
        if (!cur) return nullopt;
        if (cur->prev) cur->prev->next = cur->next;
        else           head = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
        else           tail = cur->prev;
        int val = cur->val;
        delete cur;
        return val;
    }

    int size() {
        int n = 0;
        for (Node* cur = head; cur; cur = cur->next) n++;
        return n;
    }
};
```

- The two `if/else` pairs in `removeAt` handle the head/tail edge cases — nearly every doubly-linked bug lives there.
- Fix links on both sides *before* `delete`: once freed, `n->prev`/`n->next` are unreadable.
- `insertAt(i, x)`: walk to node at `i`, insert before it with the same four-link pattern. `remove(x)`: walk comparing values, then splice out. `get(i)`: walk and return the value.
- `prepend` mirrors `append` but links at the front.

## Alternative — sentinel head/tail

- Allocate permanent dummy nodes bracketing the real elements; then `n->prev` and `n->next` are **never null** and every edge-case `if` disappears.
- `removeAt` shrinks to two unconditional lines: `n->prev->next = n->next; n->next->prev = n->prev;` — the sentinels absorb the boundaries.
- Price: two extra nodes, and the first real element is `head->next` rather than `head`.

## Alternative — XOR-linked list (memory trick)

- Store `prev XOR next` in a single pointer field; walk forward or back by XORing with the address of the previous node.
- Halves the pointer overhead; tricky to debug, illegal in C++ (pointer arithmetic on `intptr_t` is technically UB), almost never used in practice.

## Complexity

- Time: O(1) prepend, append, removeAt/remove given the node; O(n) random access and search.
- Space: O(n) plus one extra pointer per node versus a singly linked list.

## Usage

- **LRU cache**: hash map from key to node + doubly linked list for recency order; hit/evict are the O(1) node removals above.
- Browser back/forward history, music playlists with next/prev, text-editor line buffers.
- `malloc` free lists and kernel run queues splice nodes out of the middle constantly.
- Any time you need "remove the current element" in O(1) — editors, transaction logs, undo stacks.

## Cousins & contrasts

- **Singly linked list**: half the pointer traffic, but deleting a given node needs its predecessor — walk for it or keep a trailing pointer.
- **`std::list`**: the STL doubly linked list; its iterators stay valid across `splice`/`erase`, which is exactly the "O(1) removal given the node" property.
- **Array list**: O(1) indexed access but O(n) middle insert/remove — the mirror-image trade-off.
