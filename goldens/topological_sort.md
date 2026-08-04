# Topological Sort -- in-degree 0 means ready: emit, decrement neighbors, repeat

## Core idea
- A DAG always has a node with in-degree 0; emitting it is safe because all its dependencies are already out.
- Removing a node lowers its dependents' in-degree, and a count hitting 0 makes that node ready: that is Kahn's algorithm.

## Build up
1. **In-degree 0 is ready**

```
if (indeg[i] == 0) pq.push(i);
```

2. **Pop the smallest ready**

```
int u = pq.top(); pq.pop();
result.push_back(u);
```

3. **Process, decrement neighbors**

```
for (int v : adj[u])
    if (--indeg[v] == 0) pq.push(v);
```

4. **Leftovers mean a cycle**

```
if ((int)result.size() != n) return {};
```

## Diagram
```
chain: 3 -> 0 -> 1 -> 2      indeg = dependencies left
  node        0   1   2   3
  in-degree   1   1   1   0      ready (indeg 0): {3}
  pop 3 -> indeg[0] hits 0       ready: {0}
  pop 0 -> indeg[1] hits 0       ready: {1}
  pop 1 -> indeg[2] hits 0       ready: {2}
  order: 3 0 1 2                 all n emitted -> DAG
```

## Approach -- Kahn with min-heap
```cpp
using namespace std;

vector<int> topological_sort(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0);
    for (auto& e : edges) {
        adj[e.first].push_back(e.second);
        indeg[e.second]++;
    }

    priority_queue<int, vector<int>, greater<int>> pq;   // min-heap, smallest first
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0) pq.push(i);                   // step 1: ready set

    vector<int> result;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();                      // step 2: smallest ready
        result.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) pq.push(v);             // step 3: unlock dependents
    }

    if ((int)result.size() != n) return {};              // step 4: cycle
    return result;
}
```

- Steps 1-3 are the whole algorithm; the min-heap makes the order deterministic, smallest ready node first.
- Step 4's size check is the cycle test: leftover nodes never reached in-degree 0.

### Trace
- n=4, edges 0->1, 1->2, 3->0: indeg [1,1,1,0], pops 3, 0, 1, 2 -> [3,0,1,2].
- Two-node cycle 0->1, 1->0: no in-degree 0 node, result stays empty.

## Complexity
- O(V+E) time, O(V+E) space.

## Alternative -- DFS post-order
- Push u only after its children return, reverse the result; an onStack flag flags a back edge as a cycle.
- Same O(V+E), and the recursion order is the natural one for many DAG problems.

## Use when
- Dependency ordering: reach for this when tasks have prerequisites and you need a valid order (build systems, course schedules).
- Cycle detection: reach for this when leftover nodes after Kahn mean a cycle (deadlock detection).

## Cousins
- DAG shortest path: relax nodes in topological order for an O(V+E) DP.
- SCCs: Tarjan orders the condensation DAG the same way.
- Cycle detection: Kahn's leftover check, or a 3-color DFS onStack.
