# Stack

LIFO container: the last thing pushed is the first thing popped. Restricting access to **one end only** is precisely what makes every operation O(1).

## Intuition

- One index (or pointer) marks the top; push and pop both touch only that end — no traversal, no shifting.
- The restriction is the feature: LIFO order matches nested, last-opened-first-closed structure — calls, brackets, undo history.
- All ops O(1) time; O(n) space for n elements.

## Array + top index

```cpp
using namespace std;

struct Stack {
    int* data;
    int  len;     // top = len - 1
    int  cap;
};

void push(Stack& s, int x) {       // grow (double) first if s.len == s.cap
    s.data[s.len++] = x;
}

int pop(Stack& s) {                // caller checks s.len > 0
    return s.data[--s.len];
}

int peek(Stack& s) {
    return s.data[s.len - 1];
}
```

- The top is the *end* of the array — that's where arrays support O(1) add/remove; using the front would cost O(n) shifting per op.
- `pop`/`peek` on an empty stack is the one error case: guard with `len == 0`.
- A `vector<int>` with `push_back`/`pop_back`/`back` already is this.

## Alternative: linked-list stack

- Push = prepend a node, pop = unlink the head. O(1) with no capacity limit and no growth copies.
- Costs a heap allocation + pointer per element and loses cache locality; the array version wins in practice.

## Where it shows up

- The **call stack**: every function call pushes a frame, every return pops one; recursion is an implicit stack.
- Undo/redo, browser back button, DFS with an explicit stack.
- Expression work: bracket matching, infix→postfix (shunting yard), postfix evaluation.
- **Monotonic stack** problems — daily_temperatures, next greater element, largest rectangle in histogram: keep the stack strictly increasing/decreasing and pop when the invariant breaks.

## Cousins & contrasts

- **Queue**: FIFO instead of LIFO; a queue gives BFS where a stack gives DFS.
- **Recursion**: an implicit stack — any recursive algorithm can be rewritten with an explicit one (and vice versa).
- **Deque**: push/pop at both ends; a stack is a deque used at one end only.
