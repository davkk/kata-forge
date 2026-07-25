# Linked List Cycle Detection (Floyd's Tortoise & Hare)

Detect whether a linked list loops back on itself using **O(1) space** — two pointers racing at different speeds.

## Intuition

- `slow` advances 1 node per step, `fast` advances 2. If the list ends, `fast` falls off the end → no cycle.
- If there's a cycle, both pointers eventually enter it; then picture them on a circular track.
- Why they must meet: inside the loop the gap between fast and slow closes by exactly 1 node per step (fast gains 2 − 1), so it can never jump *over* slow — fast laps slow within one loop.
- O(n) time: slow is caught after at most one cycle length of extra travel; O(1) space: just two pointers, no memory of visited nodes.

## Tortoise & hare

```cpp
struct Node { int val; Node* next; };

bool has_cycle(Node* head) {
    Node *slow = head, *fast = head;
    while (fast && fast->next) {      // fast needs two hops — check both
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;                     // fast hit the end
}
```

- The loop condition is `fast && fast->next`, not `fast` — the second hop `fast->next->next` would dereference null otherwise.
- Compare pointers (`==` on addresses), never values: a cycle is about node identity.
- Empty list and single node without self-loop fall out naturally — the while condition fails immediately.

## Alternative: visited hash set

- Walk once, insert every node pointer into an `unordered_set<Node*>`; first repeat means cycle, hitting null means none.
- Same O(n) time but O(n) space — strictly worse here, though the "seen-before" pattern generalizes where two pointers can't (e.g. arbitrary graph walks).

## Extension: find the cycle entry node

- After the meeting point, reset one pointer to `head` and advance *both* by 1 — they meet exactly at the entry.
- Why: if head→entry is `a` steps and entry→meeting is `b`, then fast's doubling gives `a = remaining loop distance − b` (mod the cycle length), i.e. both pointers are equally far from the entry once one restarts at head.

## Where it shows up

- Debugging allocators / data structures: a corrupted `next` often manifests as a loop.
- Functional graphs (each x maps to f(x)): detecting repetition in pseudo-random sequences, Pollard's rho factorization.
- LeetCode-style variants: find duplicate number via index-as-pointer arrows, "happy number" detection.

## Cousins & contrasts

- **Brent's algorithm**: same O(n)/O(1) guarantees with a teleporting power-of-two probe; usually fewer pointer hops and also yields the cycle length directly.
- **Floyd middle-of-list**: same slow/fast hardware, different question — when fast falls off the end, slow sits at the midpoint.
- **Graph cycle detection**: linked lists have out-degree 1, so two pointers suffice; general graphs need DFS with a color/visited set.
