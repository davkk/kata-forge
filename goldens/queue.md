# Queue -- two ends, two pointers: FIFO needs O(1) access at opposite seams

## Core idea

- Invariant: arrival order is never broken -- head is the oldest element, tail the newest.
- Mechanism: enqueue and deque touch different, fixed ends, so one pointer per end means no search and no shifting.

## Build up

1. **One pointer can't append**

```
Node* cur = head;
while (cur->next) cur = cur->next;   // O(n) walk every time
cur->next = n;
```

2. **Keep a tail pointer**

```
tail->next = n;
```

3. **First node: both pointers**

```
if (!tail) head = n;
tail = n;
```

4. **deque: advance the head**

```
Node* n = head;
head = n->next;
if (!head) tail = nullptr;
```

## Diagram

```
enqueue 5, 7, 9           deque() -> 5            peek() -> 7
head->5 -> 7 -> 9<-tail   head->7 -> 9<-tail      head->7 (unchanged)
     |                        |
     oldest                   oldest leaves first
```

## Approach -- linked head and tail

```cpp
using namespace std;

class Queue {
public:
    Node* head;
    Node* tail;

    Queue() : head(nullptr), tail(nullptr) {}

    void enqueue(int x) {
        Node* n = new Node{x, nullptr};
        if (tail) tail->next = n;   // step 2
        else      head = n;         // step 3
        tail = n;
    }

    optional<int> deque() {
        if (!head) return nullopt;
        Node* n = head;
        head = n->next;             // step 4
        if (!head) tail = nullptr;  // drained: clear both
        int val = n->val;
        delete n;
        return val;
    }

    optional<int> peek() {
        if (!head) return nullopt;
        return head->val;
    }

    int size() {
        int len = 0;
        for (Node* cur = head; cur; cur = cur->next) len++;
        return len;
    }
};
```

- The struct is the build story: step 1's walk is replaced by `tail` (step 2), step 3 handles the empty queue, step 4 is deque.
- `peek` is `deque` minus the unlink; `size()` is an O(n) walk -- keep a counter for O(1).

### Trace

- enqueue 5, 7, 9 -> `head->5 -> 7 -> 9<-tail`; deque -> 5, then 7, then 9; a fourth deque hits `!head` and returns nullopt. Out = in.

## Complexity

- Time: O(1) per enqueue/deque/peek (size O(n) as written). Space: O(n).

## Alternative -- circular array

- head/tail become indices that advance mod cap: O(1), zero allocation, fixed memory -- the ring_buffer golden.

## Use when

- Reach for this when processing order equals arrival order: BFS, level-order, "first come, first served".
- Producer/consumer buffers, task and message queues, event loops.

## Cousins

- **Stack**: LIFO at one end -- same O(1) ops, opposite order (see stack).
- **Deque**: both ends open -- a queue is a deque used at one end.
- **Priority queue**: the most urgent leaves, not the oldest -- heap, O(log n).
- **Ring buffer**: the same FIFO in fixed memory, wrapping instead of growing.
