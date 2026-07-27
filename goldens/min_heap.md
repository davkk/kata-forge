# Min Heap

Binary heap in a flat array: a complete binary tree where **every parent ≤ its children**, so the minimum is always at index 0. Insert and extract-min both cost O(log n).

## Intuition

- Index math replaces pointers: parent of `i` is `(i-1)/2`, children are `2i+1` and `2i+2`. Only ever appending/removing at the tail keeps the tree complete (no holes).
- Heap invariant: `a[parent(i)] <= a[i]` for all `i > 0`. It's a *partial* order — only along root-to-leaf paths — which is exactly why restoring it locally is cheap.
- Insert: append (may violate the invariant against its parent), then **sift up** — swap with the parent while smaller. At most tree-height swaps → O(log n).
- Extract-min: min is at index 0. Move the last element to the root, pop the tail, **sift down** — swap with the *smaller* child until no child is smaller. Also O(log n).
- Space O(n) in one contiguous array — no pointer overhead, cache-friendly.

## Sift-up / sift-down

```cpp
using namespace std;

int parent(int i) { return (i - 1) / 2; }

void siftUp(vector<int>& h, int i) {
    while (i > 0 && h[i] < h[parent(i)]) {
        swap(h[i], h[parent(i)]);
        i = parent(i);
    }
}

void siftDown(vector<int>& h, int i) {
    int n = (int)h.size();
    while (true) {
        int l = 2*i + 1, r = 2*i + 2, smallest = i;
        if (l < n && h[l] < h[smallest]) smallest = l;
        if (r < n && h[r] < h[smallest]) smallest = r;
        if (smallest == i) break;
        swap(h[i], h[smallest]);
        i = smallest;
    }
}

void insert(vector<int>& h, int x) {
    h.push_back(x);
    siftUp(h, (int)h.size() - 1);
}

int deleteMin(vector<int>& h) {     // call only when !h.empty()
    int out = h[0];
    h[0] = h.back();
    h.pop_back();
    if (!h.empty()) siftDown(h, 0);
    return out;
}
```

- Sift down toward the *smaller* child — swapping with the larger one breaks the invariant between the two children.
- Guard the `siftDown` after `pop_back`: on a heap that just became empty, index 0 no longer exists.
- Iterative loops beat recursion here: same O(log n), no call-stack depth.

## Alternative: Floyd's build-heap — O(n) from a full array

- To heapify an existing array, sift down every node from the last parent `n/2 - 1` back to 0 — O(n) total, beating n inserts at O(n log n).
- Why it's linear: most nodes sit near the bottom, so most sift-downs travel O(1) levels; the sum of heights over all nodes is ~n, not n·log n.

```cpp
for (int i = (int)a.size() / 2 - 1; i >= 0; --i) siftDown(a, i);
```

## Where it shows up

- Dijkstra's and Prim's algorithms — the "extract cheapest frontier node" step.
- K-way merge of sorted lists/streams; every "top-k" query keeps a size-k heap.
- OS task schedulers, timer queues, discrete-event simulation (next event = min timestamp).

## Cousins & contrasts

- **Max-heap**: flip every comparison; `std::priority_queue` is a max-heap by default (`greater<>` turns it into this one).
- **std::priority_queue**: exactly this structure over a `vector` (`push`/`pop`/`top`) — hand-roll it for interviews and custom comparators.
- **d-ary heap**: d children per node → shallower tree, faster sift-up, but d comparisons per sift-down level. Wins when inserts dominate.
- **Binary search tree**: total order and O(log n) lookup of any key, but needs balancing; a heap only promises the min at the top, nothing else.
