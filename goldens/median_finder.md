# Median Finder

Running median of a stream: insert values one at a time, report the median in O(1) -- the mean of the two middle values when the count is even.

## Intuition

- Split the stream into two halves: a **max-heap `lo`** holding the lower half and a **min-heap `hi`** holding the upper half. The middle of the data lives at the two tops.
- The whole game is one invariant: **every element in `lo` is <= every element in `hi`**. Then the top(s) are the middle element(s) by construction.
- Rebalance after each insert so the sizes differ by at most 1, keeping `lo` the larger one on odd counts.
- Why heaps: O(1) access to the *extreme* of each half -- the only two elements that can possibly be the median.
- O(log n) per insert (one push + at most one rebalance pop/push), O(1) per median query.

## Approach -- two heaps

```cpp
using namespace std;

priority_queue<int> lo;                            // max-heap: lower half
priority_queue<int, vector<int>, greater<>> hi;    // min-heap: upper half

void insert(int x) {
    if (lo.empty() || x <= lo.top()) lo.push(x);
    else                             hi.push(x);
    if (lo.size() > hi.size() + 1) {                   // rebalance to <=1 apart
        hi.push(lo.top()); lo.pop();
    } else if (hi.size() > lo.size()) {
        lo.push(hi.top()); hi.pop();
    }
}

double getMedian() {
    if (lo.size() > hi.size()) return lo.top();        // odd count
    return (lo.top() + hi.top()) / 2.0;                // even count
}

int length() { return (int)(lo.size() + hi.size()); }
```

- Rebalancing moves only a root, and the root is by definition the boundary element -- the invariant survives every move.
- Decide which heap receives `x` by comparing with `lo.top()` *before* inserting; comparing against the wrong side silently breaks the invariant.
- `/2.0`, not `/2` -- integer division throws away the .5 on even counts.

## Alternative -- balanced BST / order-statistics tree

- Keeps all elements sorted with subtree sizes -> median (and any kth query, plus deletion) in O(log n).
- Strictly more general, far more code and worse constants; two heaps win when the workload is only insert + median.

## Alternative -- sort on every query

- O(n log n) per median -- the naive baseline the two heaps replace. Only sensible in toy code or as a baseline check.

## Alternative -- Fenwick tree over a small value range

- If values are integers in a small range, a Fenwick with cumulative counts can find the median in O(log V).
- O(V) space, but median query is O(1) amortized; wins for repeated percentile queries on a fixed range.

## Complexity

- Time: O(log n) per insert, O(1) per getMedian.
- Space: O(n) for both heaps combined.

## Usage

- Streaming statistics: rolling median of latencies, sensor readings, stock prices where the full history can't be stored.
- Sliding-window median -- same two-heap core plus lazy deletion of elements leaving the window.
- Percentile sketches / approximate-median structures in databases borrow the same split-at-the-middle idea.
- Any "keep a running middle of incoming data" metric in monitoring, finance, or telemetry.

## Cousins & contrasts

- **Min-heap**: the building block -- this kata is two heaps glued face-to-face.
- **Kth largest**: median is the special case k = n/2, but quickselect needs the whole array up front and mutates it -- useless on a live stream.
- **Order-statistics tree**: strictly more powerful (kth of any kind, deletion) but more code and worse constants; reach for it when you need queries other than median.
- **Sort on every query**: O(n log n) per median -- the naive baseline the two heaps replace.
