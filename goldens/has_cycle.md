# Linked List Cycle -- two pointers at different speeds: lapping proves a loop

## Core idea

- Mechanism: `slow` steps 1, `fast` steps 2; on a cycle the gap between them shrinks by exactly 1 per step, so `fast` must lap `slow` -- address equality is the proof.
- Why correct: a finite list lets `fast` fall off (no cycle); on a loop both are trapped and the gap closes 1 per step, never skipping `slow`.

## Build up

1. **One pointer walks the list**

```
ListNode* cur = head;
while (cur) cur = cur->next;
```

2. **Two pointers, different speeds**

```
ListNode *slow = head, *fast = head;
slow = slow->next;
fast = fast->next->next;
```

3. **Fast lapping slow means a cycle**

```
if (slow == fast) return true;
```

4. **The guard for fast's second hop**

```
while (fast && fast->next) { ... }
return false;
```

## Diagram

```
no cycle: 1 -> 2 -> 3 -> null       cycle 3 -> 1:
                                    1 -> 2 -> 3 -+
                                    +------------+
fast: 1, 3, null -> false
                                    step 0: s=1 f=1
                                    step 1: s=2 f=3
                                    step 2: s=3 f=2
                                    step 3: s=1 f=1  meet -> true
```

## Approach -- tortoise and hare

```cpp
using namespace std;

bool has_cycle(ListNode* head) {
    ListNode *slow = head, *fast = head;      // step 2
    while (fast && fast->next) {              // step 4
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;        // step 3
    }
    return false;                             // step 4
}
```

- The guard checks `fast && fast->next`, not just `fast`: the second hop dereferences, so odd-length lists fall off safely.
- The comparison is on addresses, never values -- duplicates are legal.

### Trace

- 1->2->3: fast hits null -> false. Add 3->1: slow 1,2,3,1... fast 1,3,2,1...; both land on node 1 at step 3 -> true.
- A self-loop meets on the first iteration.

## Complexity

- Time: O(n) -- slow is caught within one cycle length of extra travel. Space: O(1) -- two pointers.

## Alternative -- visited hash set

- Insert each pointer into an `unordered_set<ListNode*>`; first repeat = cycle, null = none. Same O(n) time, but O(n) space; generalizes to arbitrary graph walks.

## Use when

- Reach for this when you must detect a loop with zero extra memory.
- Functional graphs (x -> f(x)): Pollard's rho, repeats in pseudo-random sequences.
- Middle-of-list: the same two pointers, a different question -- when fast falls off, slow is at the midpoint.

## Cousins

- **Middle-of-list**: same slow/fast hardware, different answer.
- **Graph cycle detection**: general graphs need DFS with a visited/color set; lists have out-degree 1, so two pointers suffice.
- **Brent's algorithm**: O(1)-space variant with a power-of-two teleport, usually fewer hops, also yields cycle length.
