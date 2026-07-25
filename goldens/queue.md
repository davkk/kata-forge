# Queue

FIFO container: enqueue at the back, deque from the front — the oldest waiting item leaves first. Both ends are touched, so you need **two access points**, not one.

## Intuition

- Enqueue appends at the tail, deque removes at the head; each is a fixed amount of pointer or index work → O(1).
- A plain array + front index fails: removing from the front either shifts everything (O(n)) or lets the used window crawl off the end. Fix it with head/tail pointers or by wrapping indices (circular array).
- Guard the degenerate cases: deque/peek on empty, and (fixed capacity) enqueue on full.

## Linked list with head + tail

```cpp
using namespace std;

struct Node { int val; Node* next; };

struct Queue { Node *head, *tail; int len; };

void enqueue(Queue& q, int x) {
    Node* n = new Node{x, nullptr};
    if (q.tail) q.tail->next = n;    // link after old tail
    else        q.head = n;          // first element
    q.tail = n;
    q.len++;
}

int deque(Queue& q) {                // caller checks q.len > 0
    Node* n = q.head;
    int val = n->val;
    q.head = n->next;
    if (!q.head) q.tail = nullptr;   // just emptied — reset both ends
    q.len--;
    delete n;
    return val;
}
```

- Both pointers are required: `tail` for O(1) enqueue, `head` for O(1) deque. With only `head`, append degrades to O(n) walking.
- After removing the last node, null out **both** ends — a dangling `tail` on an empty queue is the classic bug.
- `peek` is `q.head->val`. Fixed-capacity variant: circular array with indices mod capacity (see ring_buffer).

## Alternative: two-stack queue

- Enqueue pushes onto stack `in`; deque pops from stack `out`, pouring `in` into `out` (which reverses the order) only when `out` is empty.
- Amortized O(1): each element moves `in` → `out` exactly once in its lifetime, so n operations cost O(n) total.

```cpp
vector<int> in, out;                 // in = newest on top, out = oldest on top

void enqueue(int x) { in.push_back(x); }

int deque() {
    if (out.empty())
        while (!in.empty()) { out.push_back(in.back()); in.pop_back(); }
    int val = out.back(); out.pop_back();
    return val;
}
```

## Where it shows up

- **BFS**: the frontier — nodes are expanded in discovery order, which is exactly FIFO. Level-order tree traversal is the same idea.
- Task/job schedulers, print spoolers — fair "first come, first served".
- Producer–consumer buffers, message queues, event loops serializing async work.

## Cousins & contrasts

- **Stack**: LIFO instead of FIFO; gives DFS where the queue gives BFS. Same O(1) ops, opposite order.
- **Deque**: push/pop at both ends — subsumes both stack and queue.
- **Priority queue**: the most urgent (not the oldest) leaves first; heap-backed, O(log n) per op.
- **Ring buffer**: a fixed-capacity queue that overwrites or rejects on full instead of growing.
