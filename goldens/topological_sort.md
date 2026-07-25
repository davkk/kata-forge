# Topological Sort

Linear ordering of a DAG's vertices such that every directed edge u→v has u before v. Only possible for directed acyclic graphs.

## Intuition

- A DAG always has at least one node with in-degree 0 — it has no dependencies, so it can go first.
- Remove it, decrement the in-degrees of its outgoing neighbors, repeat. This is Kahn's algorithm (BFS on in-degrees).
- If all nodes are processed, the graph is a DAG. If some remain, there is a cycle.
- O(V+E): each node enters the queue once, each edge is examined once to decrement in-degree.

## Kahn's algorithm (in-degree BFS)

```cpp
using namespace std;

vector<int> topological_sort(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0);
    for (auto& e : edges) {
        adj[e.first].push_back(e.second);
        indeg[e.second]++;
    }

    // Min-heap for deterministic order (smallest ready node first)
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0) pq.push(i);

    vector<int> result;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        result.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) pq.push(v);
    }

    if ((int)result.size() != n) return {};   // cycle
    return result;
}
```

- Decrement `indeg[v]` before checking if it reached 0 — never after, or you may skip a node.
- A plain `queue` yields a valid topological order; the min-heap guarantees determinism when the spec requires smallest-first.
- Cycle detection: `result.size() < n` implies a cycle prevented some nodes from reaching indegree 0.

## Alternative: DFS post-order

- Run DFS from every unvisited node. After all children of `u` return, prepend (or push and reverse) `u` to the result.
- Cycle detection: track an `onStack` array. If a DFS edge leads to a node that is still `onStack`, the graph has a cycle.

```cpp
vector<int> result, vis(n, 0), onStack(n, 0);
function<bool(int)> dfs = [&](int u) -> bool {
    vis[u] = onStack[u] = 1;
    for (int v : adj[u]) {
        if (onStack[v]) return false;           // back edge → cycle
        if (!vis[v] && !dfs(v)) return false;
    }
    onStack[u] = 0;
    result.push_back(u);
    return true;
};
for (int i = 0; i < n; ++i) if (!vis[i] && !dfs(i)) return {};
reverse(result.begin(), result.end());
return result;
```

## Where it shows up

- Build systems (Make, Gradle, Bazel) resolving dependency order.
- Course prerequisite scheduling, job/task sequencing.
- Instruction scheduling in compilers; Unix `tsort` utility.
- Deadlock detection: a cycle in the resource-allocation graph means deadlock.

## Cousins & contrasts

- **Kahn vs DFS post-order**: both O(V+E). Kahn is BFS-like and produces nodes in dependency-free-first order; DFS post-order reversed gives the same result but uses a stack.
- **Cycle detection**: Kahn checks `result.size() < n`; DFS checks `onStack` during recursion.
- **DAG vs tree**: a DAG allows multiple paths between nodes; a tree has exactly one. A tree is always a DAG; not every DAG is a tree.
