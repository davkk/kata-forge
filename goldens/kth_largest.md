# Kth Largest -- keep only the top k: a size-k min-heap, O(n log k)

## Core idea

- Invariant: a size-k min-heap holds the k largest elements seen; its top is the smallest of those k, which by definition is the kth largest.
- Mechanism: push, then evict when the heap exceeds k; once k larger elements exist, a smaller one can never re-enter the top k, so the heap stays exactly k large.

## Build up

1. **Size-k min-heap**

```
priority_queue<int, vector<int>, greater<>> heap;
```

2. **Evict the smallest when full**

```
heap.push(x);
if ((int)heap.size() > k) heap.pop();
```

3. **Top = kth largest**

```
return heap.top();
```

## Diagram

```
k = 2, stream [3 2 1 5 6 4], heap = top 2 seen

push 3 -> [3]                  top = 3
push 2 -> [2 3]                top = 2
push 1 -> [1 2 3] pop 1 -> [2 3]
push 5 -> [2 3 5] pop 2 -> [3 5]
push 6 -> [3 5 6] pop 3 -> [5 6]
push 4 -> [4 5 6] pop 4 -> [5 6]
                                   top = 5 = the 2nd largest
```

## Approach -- min-heap of size k

```cpp
using namespace std;

int kth_largest(const vector<int>& a, int k) {
    priority_queue<int, vector<int>, greater<>> heap;  // step 1
    for (int x : a) {
        heap.push(x);                                  // step 2
        if ((int)heap.size() > k) heap.pop();
    }
    return heap.top();                                 // step 3
}
```

- The heap never exceeds k, so every step costs O(log k), not O(log n).
- A max-heap would be wrong: the top must be the worst candidate in the top k, so it is a min-heap.

### Trace

- [3,2,1,5,6,4], k=2: heap evicts 1, then 2, then 3, then 4; it ends as [5,6] and top = 5, the 2nd largest.

## Complexity

- Time: O(n log k). Space: O(k).

## Alternative -- quickselect

- Lomuto-partition and recurse into the side containing index n - k: O(n) average, O(n^2) worst, O(1) extra space; wins for big one-shot arrays.

## Use when

- Top-k, percentile, median (k = n/2), or any kth statistic of a stream.
- Reach for this when "kth largest / smallest" and data arrives online or k is small.

## Cousins

- Sort then index: a[n - k] after sorting, simplest correct answer at O(n log n).
- Quickselect: the one-sided partition recursion, see alternative.
- Quicksort: the same partition; quicksort recurses both sides, quickselect only one.
- Median of medians: the theoretical worst-case O(n) fix for quickselect.
