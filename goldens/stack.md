# Stack

LIFO container: the last thing pushed is the first thing popped. Restricting access to **one end only** is precisely what makes every operation O(1).

## Intuition

- One pointer marks the top; push and pop both touch only that end — no traversal, no shifting.
- The restriction is the feature: LIFO order matches nested, last-opened-first-closed structure — calls, brackets, undo history.
- All ops O(1) time; O(n) space for n elements.

## Approach 1 — linked-node stack

```cpp
using namespace std;

struct Stack {
    Node *head = nullptr;

    void push(int x) {
        head = new Node{x, head};
    }

    optional<int> pop() {
        if (!head) return nullopt;
        Node* n = head;
        int val = n->val;
        head = n->next;
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

- The top of the stack is the **head** of the list — the one end where a singly-linked list gives O(1) insert and remove. Push prepends, pop unlinks.
- One pointer suffices; a queue needs two (head + tail) because it touches both ends.
- `pop`/`peek` return `nullopt` on empty — the caller owns the error check. `delete` the unlinked node or you leak.
- `size()` is O(n) as a traversal; keep a running counter (++ on push, -- on pop) for O(1).

## Approach 2 — array-backed stack

```cpp
vector<int> data;

void push(int x) { data.push_back(x); }

optional<int> pop() {
    if (data.empty()) return nullopt;
    int val = data.back();
    data.pop_back();
    return val;
}
```

- The top is the *end* of the vector — the only end where arrays give O(1) add/remove; the front would cost O(n) shifting per op.
- Wins in practice: no per-element allocation, cache-friendly, amortized O(1) growth. The linked version pays a heap allocation + pointer per element.

## Alternative — two-stack queue (the reverse trick)

- Two stacks simulate a FIFO queue: enqueue pushes onto `in`, dequeue pops from `out` (pouring `in` into `out` when empty). See queue.md.

## Complexity

- Time: O(1) per push, pop, peek, size (amortized growth for the array version).
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
