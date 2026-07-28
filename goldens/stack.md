# Stack

LIFO container: the last thing pushed is the first thing popped. Restricting access to **one end only** is precisely what makes every operation O(1).

## Intuition

- One index (or pointer) marks the top; push and pop both touch only that end — no traversal, no shifting.
- The restriction is the feature: LIFO order matches nested, last-opened-first-closed structure — calls, brackets, undo history.
- All ops O(1) time; O(n) space for n elements.

## Approach — array-backed stack

```cpp
using namespace std;

struct Stack {
    vector<int> data;

    void push(int x) {
        data.push_back(x);
    }

    optional<int> pop() {
        if (data.empty()) return nullopt;
        int val = data.back();
        data.pop_back();
        return val;
    }

    optional<int> peek() {
        if (data.empty()) return nullopt;
        return data.back();
    }

    int size() {
        return data.size();
    }
};
```

- The top is the *end* of the vector — that's where arrays support O(1) add/remove; using the front would cost O(n) shifting per op.
- `pop`/`peek` return `nullopt` on empty — the caller owns the error check.
- A raw array + top index works identically; `vector` just handles growth.

## Alternative — linked-list stack

- Push = prepend a node, pop = unlink the head. O(1) with no capacity limit and no growth copies.
- Costs a heap allocation + pointer per element and loses cache locality; the array version wins in practice.

## Alternative — two-stack queue (the reverse trick)

- Two stacks can simulate a FIFO queue: enqueue pushes onto `in`, dequeue pops from `out` (pouring `in` into `out` when empty). See queue.md.
- The same composition lets a stack stand in for a deque when you only need one end.

## Complexity

- Time: O(1) per push, pop, peek, size.
- Space: O(n) for n elements.

## Usage

- **The call stack**: every function call pushes a frame, every return pops one; recursion is an implicit stack.
- Undo/redo, browser back button, DFS with an explicit stack.
- Expression work: bracket matching, infix to postfix (shunting yard), postfix evaluation.
- **Monotonic stack** problems — daily temperatures, next greater element, largest rectangle in histogram: keep the stack strictly increasing/decreasing and pop when the invariant breaks.
- Any "remember what just happened and undo it" pipeline.

## Cousins & contrasts

- **Queue**: FIFO instead of LIFO; a queue gives BFS where a stack gives DFS.
- **Recursion**: an implicit stack — any recursive algorithm can be rewritten with an explicit one (and vice versa).
- **Deque**: push/pop at both ends; a stack is a deque used at one end only.
