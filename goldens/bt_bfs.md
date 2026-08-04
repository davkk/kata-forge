# BFS on a Binary Tree -- a FIFO queue drains one level before the next

## Core idea

- Invariant: everything in the queue at the start of a pop belongs to a single depth level, because children are appended at the tail, behind every node already waiting.
- Mechanism: FIFO ordering IS depth ordering -- each node's children queue up after all pending nodes, so the front always pops the shallowest remaining node.

## Build up

1. **A queue holds the frontier**

```
queue<Node*> q;
q.push(root);
```

2. **Pop the front, check it**

```
Node* curr = q.front(); q.pop();
if (curr->val == needle) return true;
```

3. **Children join the back**

```
if (curr->left)  q.push(curr->left);
if (curr->right) q.push(curr->right);
```

4. **Snapshot the level before draining**

```
int level_size = (int)q.size();   // before children arrive
for (int i = 0; i < level_size; ++i) ...
```

## Diagram

```
         A <-- root
        / \
       B   C
      / \   \
     D   E   F

queue: [A]        pop A, push B C
       [B C]      pop B, push D E
       [C D E]    pop C, push F
       [D E F]    pop D E F

order: A B C D E F -- strictly level by level
```

## Approach -- level-order search

```cpp
using namespace std;

bool bfs(Node* root, int needle) {
    if (!root) return false;
    queue<Node*> q;
    q.push(root);                           // step 1
    while (!q.empty()) {
        int level_size = (int)q.size();     // step 4: width of this level
        for (int i = 0; i < level_size; ++i) {
            Node* curr = q.front(); q.pop();    // step 2
            if (curr->val == needle) return true;
            if (curr->left)  q.push(curr->left);    // step 3
            if (curr->right) q.push(curr->right);   // step 3
        }
        // one full level done here -- collect averages, maxima, ...
    }
    return false;
}
```

- The `while` just drains what steps 1-3 build; step 4's snapshot turns a plain search into per-level work.
- `level_size` is read once, before the loop -- the current frontier, not the growing tail.
- Only non-null children are enqueued, so the null check lives in one place.

### Trace

- Root 20, children 10/50, 45 under 30 (right of 50): pop 20, push 10 50; pop 10, 50 (push 5 15 30 100); pop 5,15,30 (push 29 45); pop 29, then 45 -> true.
- Levels drain one at a time; the hit at 45 exits mid-level, so unvisited nodes stay queued.

## Complexity

- Time: O(n) -- each node enqueued once, popped once. Space: O(w), w = widest level.

## Alternative -- DFS with depth parameter

- Recurse carrying a `depth` and append to `levels[depth]`: same per-level result with O(h) space, but no queue and no free left-to-right guarantee.

## Use when

- Reach for this when the question is about a node's *layer*: k-th row, shortest path, level order output.
- Level-grouped results: averages, maxima, right-side view (last node per level), zigzag.
- The answer sits near the root: BFS finds the shallowest hit first, DFS does not.

## Cousins

- **DFS**: a stack, O(h) space -- for paths and bottom-up work, not layers.
- **Graph BFS**: the same queue plus a `visited` set; trees need none.
- **dfs_on_bst**: prunes by ordering to O(h); BFS has no ordering to exploit.
- **Dijkstra**: BFS whose FIFO becomes a min-heap on distance.
