# Doubly Linked List

Each node points both ways (`prev` and `next`). The payoff over a singly linked list: **O(1) deletion given only the node** — no walk to find its predecessor.

## Intuition

- Singly linked removal needs the target's predecessor to reroute `prev->next`. With `prev` pointers, the node itself carries both neighbors: two assignments unlink it.
- Keep both `head` and `tail`: append and prepend both become O(1), no walking.
- Cost: one extra pointer per element, and every splice has twice as many assignments to get right (4 links for insert, 2 for delete).

## Approach — one unlink routine, everything else finds nodes

```cpp
using namespace std;

struct DoublyLinkedList {
    Node *head = nullptr, *tail = nullptr;

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

    void insertAt(int i, int x) {
        if (i == 0) { prepend(x); return; }
        Node* cur = head;
        for (int j = 0; cur && j < i; j++) cur = cur->next;
        if (!cur) { append(x); return; }
        Node* n = new Node{x, cur->prev, cur};
        if (cur->prev) cur->prev->next = n;
        cur->prev = n;
    }

    void drop(Node* cur) {  // unlink + free; the one place head/tail edge cases live
        if (cur->prev) cur->prev->next = cur->next;
        else           head = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
        else           tail = cur->prev;
        delete cur;
    }

    optional<int> removeAt(int i) {
        Node* cur = head;
        for (int j = 0; cur && j < i; j++) cur = cur->next;
        if (!cur) return nullopt;
        int val = cur->val;
        drop(cur);
        return val;
    }

    optional<int> remove(int x) {
        for (Node* cur = head; cur; cur = cur->next)
            if (cur->val == x) { drop(cur); return x; }
        return nullopt;
    }

    optional<int> get(int i) {
        Node* cur = head;
        for (int j = 0; cur && j < i; j++) cur = cur->next;
        if (!cur) return nullopt;
        return cur->val;
    }

    int size() {
        int n = 0;
        for (Node* cur = head; cur; cur = cur->next) n++;
        return n;
    }
};
```

- `drop` is the single unlink routine — the head/tail edge cases live there and nowhere else; `removeAt`/`remove` only find the node.
- Fix links on both sides *before* `delete`: once freed, the pointers are unreadable.
- `insertAt(i, x)` falls back to `prepend`/`append` at the boundaries.

## Alternative — sentinel head/tail

- Permanent dummy nodes bracket the real elements so `n->prev`/`n->next` are never null — the edge-case `if`s in `drop` vanish, at the price of two extra nodes.

## Complexity

- O(1) prepend/append/removeAt/remove; O(n) access/search. O(n) space plus one pointer per node.

## Usage

- **LRU cache**: hash map to node + doubly linked list for recency order; hit/evict are O(1) removals.
- Browser back/forward, playlists, text-editor buffers, kernel run queues.

## Cousins & contrasts

- **Singly linked list**: half the pointer traffic, but deleting a node needs its predecessor — `std::forward_list` vs `std::list`.
