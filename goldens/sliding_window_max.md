# Sliding Window Maximum -- max of each k-window in O(n): a deque in decreasing value order, stale fronts expired

## Core idea

- Invariant: the deque holds only candidates for future maxima -- indices whose values strictly decrease; the front is the current window's max.
- Mechanism: a new value kills every smaller tail value (the newcomer is larger and outlives them all), and the window's left edge kills expired fronts. Each index enters and leaves once.

## Build up

1. **Max-heap per window**

```
push k, take top, slide     // O(n log k), lazy expiry
```

2. **Deque: decreasing values**

```
while (a[dq.back()] < a[i]) dq.pop_back();   // smaller tails die
```

3. **Expire stale indices**

```
if (dq.front() <= i - k) dq.pop_front();     // left the window
```

4. **Front is the max**

```
if (i >= k - 1) out.push_back(a[dq.front()]);
```

## Diagram

```
a = [1 3 -1 -3 5 3 6 7], k = 3
i=1   3    dq [1]          3 pops 0
i=2  -1    dq [1 2]        max 3
i=3  -3    dq [1 2 3]      max 3
i=4   5    dq [] [4]       5 pops 3, 2, 1
i=5   3    dq [4 5]        max 5
i=6   6    dq [6]          6 pops 5, 4
i=7   7    dq [7]          7 pops 6
```

## Approach -- monotonic deque

```cpp
using namespace std;

vector<int> sliding_window_max(const vector<int>& a, int k) {
    vector<int> out;
    deque<int> dq;
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] < a[i])   // step 2: kill smaller
            dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k)                     // step 3: expire stale
            dq.pop_front();
        if (i >= k - 1)                              // step 4: front is max
            out.push_back(a[dq.front()]);
    }
    return out;
}
```

- Order matters: evict smaller backs, push, expire the front, then emit.
- `<` (not `<=`) when evicting keeps equal values as fresh candidates; both are correct.
- The first k - 1 steps only warm the deque; output starts at i == k - 1.

### Trace

- `[1 3 -1 -3 5 3 6 7], k = 3` -> `[3 3 5 5 6 7]`; 5 at i=4 pops the whole deque in one go.

## Complexity

- Time: O(n) amortized. Space: O(k) for the deque.

## Alternative -- max-heap per window

- A max-heap of (value, index) with lazy deletion of expired tops: O(n log k), simpler to reason about but slower.

## Use when

- Live-stream maxima: max price, temperature, throughput over the last k samples.
- Reach for this when every contiguous k-window needs its max or min and the window slides one step at a time.
- Flip the comparison and the same algorithm answers sliding-window minimum.

## Cousins

- **Daily temperatures**: the same monotonic idea with a stack for next-greater distances.
- **Monotonic queue**: the idea itself: value order, not insertion order, drives eviction.
- **Max-heap per window**: O(log k) per step with lazy deletion: strictly worse.
- **Prefix maxima**: only for windows anchored at 0; cannot shrink the start.
