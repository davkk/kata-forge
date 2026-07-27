# Breadth-First Search (Level Order)

Breadth-first traversal visits the tree **level by level, left to right**, using a queue. DFS dives with a stack; BFS fans out — a node is never visited before every node on the levels above it.

## Intuition

- A FIFO queue is the whole trick: children enqueued now are processed only after every node already waiting — which is exactly the current level.
- While the DFS orders differ by *when* you visit a node relative to its children, BFS sidesteps the question: visit order is determined purely by depth.
- Snapshot `q.size()` at the start of each level to know where one level ends and the next begins — that single counter unlocks all per-level problems.
- Time O(n). Space O(w), where w is the maximum tree width — a complete tree's last level holds ~n/2 nodes, so BFS can use far more memory than DFS's O(h).

## Level-order search

```cpp
using namespace std;

struct Node { int val; Node* left; Node* right; };

bool bfs(Node* root, int needle) {
    if (!root) return false;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        int level_size = (int)q.size();          // nodes on this level
        for (int i = 0; i < level_size; ++i) {
            Node* curr = q.front(); q.pop();
            if (curr->val == needle) return true;
            if (curr->left)  q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        // one full level done here — collect averages, maxima, …
    }
    return false;
}
```

- Only enqueue non-null children — keeps all null checks in one place.
- The `level_size` loop is optional for a plain search but is the standard idiom whenever results are grouped per level.

## Where it shows up

- **Shortest depth / nearest match**: the first BFS hit is guaranteed shallowest — DFS gives no such guarantee.
- Level averages, level maxima, right-side view (last node per level), zigzag traversal (alternate direction per level).
- Serialization formats that store a tree level by level.

## Cousins & contrasts

- **DFS (pre/in/post-order)**: dives to a leaf with a stack or recursion, O(h) space; better for structure-preserving walks and bottom-up aggregation.
- **Graph BFS**: same queue plus a `visited` set — trees need none since they have no cycles.
- **Dijkstra**: BFS generalized to weighted graphs — the FIFO queue becomes a min-heap on distance.
