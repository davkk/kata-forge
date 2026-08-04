# Min Heap -- parent <= child in a packed array: the minimum lives at index 0

## Core idea

- Invariant: every parent is <= its children -- a partial order that constrains root-to-leaf chains only, never siblings, so one misplaced node is fixed by sifting along a single path.
- The array *is* the tree: index math `(i-1)/2`, `2i+1`, `2i+2` replaces pointers, and only tail edits (append, last-to-root + pop) keep it complete -- no rebalancing, ever.

## Build up

1. **Insert: append, sift up**

```
data.push_back(x);                                  // shape: tree stays complete
for (int i = last; i > 0 && data[i] < data[parent(i)]; i = parent(i))
    swap(data[i], data[parent(i)]);                 // order: walk up one path
```

2. **DeleteMin: last element to the root**

```
int out = data[0];
data[0] = data.back();
data.pop_back();                                    // shape: tail gone, still complete
```

3. **Sift down into the smaller child**

```
int l = 2*i + 1, r = 2*i + 2, smallest = i;
if (l < n && data[l] < data[smallest]) smallest = l;
if (r < n && data[r] < data[smallest]) smallest = r;
if (smallest == i) break;                           // both children bigger: done
```

## Diagram

```
insert 1:  [3,5,9] --append--> [3,5,9,1] --sift up--> [1,5,9,3]
tree:         3                       1
             / \                     / \
            5   9                   5   9
                                  /
                                 1
deleteMin:  save 1, root = 3, pop tail -> [3,5,9]   3 < 5, 9: stop
```

## Approach -- sift-up / sift-down

```cpp
using namespace std;

class MinHeap {
public:
    vector<int> data;

    MinHeap() : data() {}

    int parent(int i) { return (i - 1) / 2; }           // index math: 2i+1, 2i+2

    void siftUp(int i) {
        while (i > 0 && data[i] < data[parent(i)]) {    // step 1: order repair
            swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int n = (int)data.size();
        while (true) {
            int l = 2*i + 1, r = 2*i + 2, smallest = i; // step 3: smaller child
            if (l < n && data[l] < data[smallest]) smallest = l;
            if (r < n && data[r] < data[smallest]) smallest = r;
            if (smallest == i) break;                   // step 3: done
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

    void insert(int x) {
        data.push_back(x);                              // step 1: shape
        siftUp((int)data.size() - 1);                   // step 1: order
    }

    optional<int> deleteMin() {
        if (data.empty()) return nullopt;
        int out = data[0];
        data[0] = data.back();                          // step 2: shape
        data.pop_back();
        if (!data.empty()) siftDown(0);                 // step 3: order
        return out;
    }

    int size() { return (int)data.size(); }
};
```

- Steps 1-3 assembled: append + siftUp (1), last-to-root + pop (2), siftDown (3); the `n` bounds guard a missing child near the tail.
- siftDown must pick the *smaller* child -- swapping into the larger one breaks the invariant between siblings.

### Trace

- insert 3, 5, 9 -> [3,5,9]; insert 1 -> append [3,5,9,1], sift up past 3 -> [1,5,9,3].
- deleteMin -> save 1, root = 3, pop tail -> [3,5,9]; sift down: 3 < 5, 9 -> stop. Returns 1.

## Complexity

- Time: O(log n) per insert/deleteMin. Space: O(n) in one array.

## Alternative -- Floyd build-heap

- Sift down from `n/2 - 1` down to 0 heapifies a whole array in O(n): half the nodes are leaves, and the sum of all sift heights is ~n, not n*log n.

## Use when

- Reach for this when you repeatedly need the min (or max) of a growing set -- top-k, cheapest-next, running median (see median_finder).
- Task queues, Dijkstra/Prim frontier extraction, k-way merges: "give me the extreme" workloads.
- Never to search or remove arbitrary elements -- the top is the only window.

## Cousins

- **Priority queue**: `std::priority_queue` is exactly this over a vector (`greater<>` flips it to a min-heap).
- **BST**: total order with search of any key, but needs balancing; a heap only promises the top.
- **d-ary heap**: d children per node -- shallower tree, faster sifts, more comparisons per level.
- **heapselect**: top-k of an array in O(n) -- this heap is its incremental, insert-friendly form.
