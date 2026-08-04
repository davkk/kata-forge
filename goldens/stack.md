# Stack -- one end only: LIFO makes push/pop/peek O(1)

## Core idea

- Only the **top** is reachable; work happens at one fixed end, so every op is pointer rewiring -- no search, no shift.

## Build up

1. **One pointer for the top**

```
class Stack {
public:
    Node* head;
    Stack() : head(nullptr) {}
};
```

2. **push: the new top points at the old top**

```
head = new Node{x, head};
```

3. **pop: the top moves down**

```
Node* n = head;
head = n->next;
delete n;
```

4. **empty: nothing to pop**

```
if (!head) return nullopt;
```

## Diagram

```
push 1, 2, 3        pop() -> 3         peek() -> 2
top->3              top->2             top->2
     |->2                |->1               |->1 (no change)
     |->1
```

## Approach -- linked nodes

```cpp
using namespace std;

class Stack {
public:
    Node* head;

    Stack() : head(nullptr) {}

    void push(int x) {
        head = new Node{x, head};
    }

    optional<int> pop() {
        if (!head) return nullopt;
        Node* n = head;
        head = n->next;
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

- The struct is build steps 1-4 assembled: `head` (step 1), push (step 2), pop = step 3 + step 4's guard.
- `peek` is `pop` without the unlink; `size()` is an O(n) walk -- keep a counter for O(1).

### Trace

- push 5, 7, 9 -> `9 -> 7 -> 5`; pop -> 9, 7, 5, then nullopt. Out = reverse of in.

## Complexity

- Time: O(1) per push/pop/peek (size O(n) without counter). Space: O(n).

## Alternative -- array-backed

- Top = the vector's **end**: `push_back`/`pop_back` -- no per-node allocation, cache-friendly (what `std::stack` does).

## Use when

- "Most recent first": brackets, postfix, DFS, backtracking, undo history.
- **Monotonic stack** (next-greater, daily temperatures): reach for this when you need the nearest smaller/larger neighbor -- LIFO plus a value rule; each element enters/leaves once -> whole pass O(n).

## Cousins

- **Queue**: FIFO needs two ends -> head + tail (see queue).
- **Recursion**: an implicit stack; any recursive algorithm rewrites to explicit.
- **Deque**: both ends; a stack is a deque used at one end only.
