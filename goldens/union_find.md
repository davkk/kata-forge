# Union-Find -- parent-pointer forests: path compression plus union by size give near-O(1) ops
## Core idea
- Invariant: each set is a tree whose root represents it; `p[x]` points toward the root, and a root stores its size as `-size`, so `p[root] < 0` marks roots.
- Mechanism: `find` walks to the root and compresses the visited path; `unite` hangs the smaller root under the larger -- each alone is O(log n), together O(alpha(n)) amortized.
## Build up
1. **Naive parent array**
```
p[x] = x;                       // every element its own root
```
2. **find walks to the root**
```
while (p[x] != x) x = p[x];     // the root is the representative
```
3. **unite: small root under big**
```
rx = find(x), ry = find(y);
if (sz[rx] < sz[ry]) swap(rx, ry);
p[ry] = rx;  sz[rx] += sz[ry];
```
4. **Path compression**
```
if (p[x] != x) p[x] = find(p[x]);   // next find is O(1)
```
## Diagram
```
parent:  [-1, -1, -1, -1, -1]         start: all roots, size 1
unite(0,1)   p = [-2, 0, -1, -1, -1]  0 <- 1
unite(2,3)   p = [-2, 0, -2, 2, -1]   0 <- 1    2 <- 3
unite(1,3)   sizes 2 vs 2, 0 wins     0 <- 1
             p = [-4, 0, 0, 2, -1]      <- 2 <- 3
find(3): 3 -> 2 -> 0, compress: p[3] = 0
```
## Approach -- negative-size parent
```cpp
using namespace std;

class UnionFind {
public:
    vector<int> p;

    UnionFind(int n) : p(n, -1) {}

    int find(int x) {
        return p[x] < 0 ? x : (p[x] = find(p[x])); // step 4: compress
    }

    bool unite(int x, int y) {
        x = find(x), y = find(y);                   // step 2
        if (x == y) return false;
        if (p[x] > p[y]) swap(x, y);                // step 3: negative sizes, bigger first
        p[x] += p[y];
        p[y] = x;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    int size() { return (int)p.size(); }
};
```
- `p[root] < 0` marks a root and stores `-size` there, so step 3's `sz[]` collapses into the same array.
- `unite` returns true only when a merge happens -- decrement a counter on success to track components.
- `connected` is just "same root"; `size()` is the element count.
### Trace
- n=5: unite(0,1) -> p = [-2,0,-1,-1,-1]; unite(2,3) -> [-2,0,-2,2,-1]; unite(1,3) -> 0 wins, p = [-4,0,0,2,-1].
- connected(0,3): find(3) climbs 3 -> 2 -> 0 and compresses p[3] = 0; connected(0,4) -> false.
## Complexity
- Time: O(alpha(n)) amortized per op. Space: O(n) parent array.
## Alternative -- union by rank
- Rank bounds tree height instead of size; identical alpha(n) bound, textbook variant -- size reads more naturally.
## Use when
- Reach for this when: you only ever merge sets and ask connectivity (never split) -- dynamic connectivity, Kruskal's MST cycle checks.
- The edge whose `unite` returns false closes a cycle (Graph Valid Tree); deletions are answerable by reversing time.
## Cousins
- **BFS/DFS**: O(V+E) per connectivity question vs near-O(1) amortized per union/find.
- **Path compression alone / union by size alone**: each O(log n); alpha(n) needs both (Tarjan, 1975).
- **Euler tour tree**: dynamic connectivity with deletions -- far heavier.
