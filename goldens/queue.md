# Queue

FIFO container: enqueue at the back, dequeue from the front — the oldest waiting item leaves first. Both ends are touched, so you need **two access points**, not one.

## Intuition

- Enqueue appends at the tail, dequeue removes at the head; each is a fixed amount of pointer or index work -> O(1).
- A plain array + front index fails: removing from the front either shifts everything (O(n)) or lets the used window crawl off the end. Fix it with head/tail pointers or by wrapping indices (circular array).
- Guard the degenerate cases: dequeue/peek on empty.

## Approach 1 — linked list with head + tail

```cpp
using namespace std;

struct Queue {
    Node *head, *tail;

    void enqueue(int x) {
        Node* n = new Node{x, nullptr};
        if (tail) tail->next = n;
        else      head = n;
        tail = n;
    }

    optional<int> deque() {
        if (!head) return nullopt;
        Node* n = head;
        int val = n->val;
        head = n->next;
        if (!head) tail = nullptr;
        delete n;
        return val;
    }

    optional<int> peek() {
        if (!head) return nullopt;
        return head->val;
    }

    int length() {
        int len = 0;
        for (Node* cur = head; cur; cur = cur->next) len++;
        return len;
    }
};
```

- Both pointers are required: `tail` for O(1) enqueue, `head` for O(1) dequeue. With only `head`, append degrades to O(n) walking.
- After removing the last node, null out **both** ends — a dangling `tail` on an empty queue is the classic bug.
- `length()` is O(n) with this naïve traversal; store a running `len` counter (increment on enqueue, decrement on deque) for O(1).

## Approach 2 — two-stack queue

```cpp
vector<int> in, out;                 // in = newest on top, out = oldest on top

void enqueue(int x) { in.push_back(x); }

int dequeue() {
    if (out.empty())
        while (!in.empty()) { out.push_back(in.back()); in.pop_back(); }
    int val = out.back(); out.pop_back();
    return val;
}
```

- Enqueue pushes onto stack `in`; dequeue pops from stack `out`, pouring `in` into `out` (which reverses the order) only when `out` is empty.
- Amortized O(1): each element moves `in` -> `out` exactly once in its lifetime, so n operations cost O(n) total.

## Alternative — circular array (fixed capacity)

- `head`/`tail` indices advance and wrap via `(i + 1) % cap`. The ring buffer golden has the full version.
- Wins on cache locality and no allocation; fails the moment you need a capacity that grows.

## Complexity

- Time: O(1) per enqueue, deque, peek (amortized for the two-stack version).
- Space: O(n).

## Usage

- **BFS**: the frontier — nodes are expanded in discovery order, which is exactly FIFO. Level-order tree traversal is the same idea.
- Task/job schedulers, print spoolers — fair "first come, first served".
- Producer/consumer buffers, message queues, event loops serializing async work.
- Any place where fairness or arrival order matters: ticket lines, OS ready queues, network packet scheduling.

## Cousins & contrasts

- **Stack**: LIFO instead of FIFO; gives DFS where the queue gives BFS. Same O(1) ops, opposite order.
- **Deque**: push/pop at both ends — subsumes both stack and queue.
- **Priority queue**: the most urgent (not the oldest) leaves first; heap-backed, O(log n) per op.
- **Ring buffer**: a fixed-capacity queue that overwrites or rejects on full instead of growing.
