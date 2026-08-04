# Median Finder -- two facing heaps: the middle lives at the two tops

## Core idea

- Split invariant: max-heap `lo` holds the lower half, min-heap `hi` the upper half, every `lo` value <= every `hi` value, and sizes differ by at most 1 -- the two tops border the middle of the data.
- Mechanism: the median is an order statistic; a heap exposes its extreme in O(1), so each side surfaces its middle candidate with no interior ordering.

## Build up

1. **Two heaps face each other**

```
priority_queue<int> lo;                              // max-heap: lower half
priority_queue<int, vector<int>, greater<>> hi;      // min-heap: upper half
```

2. **Route by lo's top**

```
if (lo.empty() || val <= lo.top()) lo.push(val);
else                               hi.push(val);
```

3. **Rebalance: move one root across**

```
if (lo.size() > hi.size() + 1) { hi.push(lo.top()); lo.pop(); }
else if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
```

4. **Read the median from the tops**

```
if (lo.size() > hi.size()) return lo.top();
return (lo.top() + hi.top()) / 2.0;      // /2.0, not /2: keep the .5
```

## Diagram

```
insert 5, 10, 3, 8, 1     lo (max)      hi (min)      median
  5                         [5]           []           5.0
  10 -> hi                  [5]           [10]         7.5
  3 -> lo                   [3,5]         [10]         5.0
  8 -> hi                   [3,5]         [8,10]       6.5
  1 -> lo                   [1,3,5]       [8,10]       5.0
  lo.top() <= hi.top() holds after every step
```

## Approach -- two heaps

```cpp
using namespace std;

class MedianFinder {
public:
    priority_queue<int> lo;                                  // step 1: max-heap
    priority_queue<int, vector<int>, greater<>> hi;          // step 1: min-heap

    MedianFinder() : lo(), hi() {}

    void insert(int val) {
        if (lo.empty() || val <= lo.top()) lo.push(val);     // step 2: route
        else                               hi.push(val);
        if (lo.size() > hi.size() + 1) {                     // step 3: rebalance
            hi.push(lo.top()); lo.pop();
        } else if (hi.size() > lo.size()) {
            lo.push(hi.top()); hi.pop();
        }
    }

    double getMedian() {
        if (lo.size() > hi.size()) return lo.top();          // step 4
        return (lo.top() + hi.top()) / 2.0;
    }

    int size() { return (int)(lo.size() + hi.size()); }
};
```

- Steps 1-4 assembled: facing heaps (1), routing by `lo.top()` (2), one-root rebalance (3), top read (4).
- The rebalance is the correctness lever: skipping it lets a value sit on the wrong side and corrupts the median.

### Trace

- insert 5,10,3,8,1 -> medians 5.0, 7.5, 5.0, 6.5, 5.0; `lo.top() <= hi.top()` holds after every step.
- insert 100 -> hi takes it: [8,10,100]; lo=3, hi=3 -> median = (5+8)/2 = 6.5.

## Complexity

- Time: O(log n) per insert, O(1) per getMedian. Space: O(n).

## Alternative -- order-statistics tree

- A balanced BST with subtree sizes answers any kth query (and deletion) in O(log n) -- more general, far more code; two heaps win for insert + median only.

## Use when

- Reach for this when the pattern is "insert one element, immediately report the middle" on an unbounded stream.
- Rolling median of latencies or sensor readings; sliding-window median with lazy deletion.

## Cousins

- **Min-heap**: the building block -- this kata is two heaps glued face to face.
- **Kth largest**: median is k = n/2; quickselect needs the whole array and mutates it.
- **Sort on every query**: O(n log n) per median -- the naive baseline this replaces.
- **Fenwick over a value range**: O(log V) median for bounded integer streams.
