# Doubly Linked List

Each node points both ways (`prev` and `next`). The payoff over a singly linked list: **O(1) deletion given only the node** — no walk to find its predecessor.

## Intuition

- Singly linked removal needs the target's predecessor to reroute `prev->next`. With `prev` pointers, the node itself carries both neighbors: two assignments unlink it.
- Keep both `head` and `tail`: append and prepend both become O(1), no walking.
- Cost: one extra pointer per element, and every splice has twice as many assignments to get right (4 links for insert, 2 for delete).
- Indexed access is still O(n) — pointers buy splicing speed, not search speed.

## Splice out a node (the reason for prev)

```cpp
using namespace std;

struct Node { int val; Node *prev, *next; };

struct List { Node *head, *tail; int len; };

void removeNode(List& l, Node* n) {         // O(1); n must belong to l
    if (n->prev) n->prev->next = n->next;   // bypass from the left
    else         l.head = n->next;          // n was the head
    if (n->next) n->next->prev = n->prev;   // bypass from the right
    else         l.tail = n->prev;          // n was the tail
    delete n;
    l.len--;
}

void append(List& l, int x) {
    Node* n = new Node{x, l.tail, nullptr};
    if (l.tail) l.tail->next = n;           // link old tail forward
    else        l.head = n;                 // first node
    l.tail = n;
    l.len++;
}
```

- The two `if/else` pairs in `removeNode` are the head/tail edge cases — nearly every doubly-linked bug lives there.
- Fix links on both sides *before* `delete`: once freed, `n->prev`/`n->next` are unreadable.
- `insertAt(x, i)`: walk to the node at `i`, insert before it with the same four-link pattern (`n->prev->next`, `n->prev`, `cur->prev->next`, `cur->prev`).

## Alternative: sentinel head/tail

- Allocate permanent dummy nodes bracketing the real elements; then `n->prev` and `n->next` are **never null** and every edge-case `if` disappears.
- `removeNode` shrinks to two unconditional lines: `n->prev->next = n->next; n->next->prev = n->prev;` — the sentinels absorb the boundaries.
- Price: two extra nodes, and the first real element is `head->next` rather than `head`.

```cpp
Node* head = new Node{0, nullptr, nullptr};   // sentinels
Node* tail = new Node{0, nullptr, nullptr};
head->next = tail;
tail->prev = head;                            // empty list: sentinels pointing at each other
```

## Where it shows up

- **LRU cache**: hash map from key to node + doubly linked list for recency order; hit/evict are the O(1) node removals above.
- Browser back/forward history, music playlists with next/prev, text-editor line buffers.
- `malloc` free lists and kernel run queues splice nodes out of the middle constantly.

## Cousins & contrasts

- **Singly linked list**: half the pointer traffic, but deleting a given node needs its predecessor — walk for it or keep a trailing pointer.
- **std::list**: the STL doubly linked list; its iterators stay valid across `splice`/`erase`, which is exactly the "O(1) removal given the node" property.
- **Array list**: O(1) indexed access but O(n) middle insert/remove — the mirror-image trade-off.
