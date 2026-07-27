# Sliding Window Maximum

Maximum of every contiguous window of size k, in O(n) total -- a **monotonic deque** instead of rescanning each window (O(n*k)).

## Intuition

- Maintain a deque of **indices** whose values are strictly decreasing; the front is always the current window's maximum.
- When a new element arrives, pop from the back everything smaller than it: those can *never* be the max again -- the newcomer is larger and outlives them in every future window.
- Pop from the front any index that has left the window (`<= i - k`).
- Each index is pushed once and popped at most once -> amortized O(1) per step, O(n) total. The deque holds at most k entries -> O(k) space.
- Mental model: the deque is the list of *candidates* for future maxima, decreasing in value and increasing in age.

## Approach -- monotonic deque

```cpp
using namespace std;

vector<int> max_sliding_window(const vector<int>& a, int k) {
    vector<int> out;
    deque<int> dq;
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] < a[i])
            dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k)
            dq.pop_front();
        if (i >= k - 1)
            out.push_back(a[dq.front()]);
    }
    return out;
}
```

- Store **indices, not values** -- window expiry (`i - k`) cannot be checked from values alone. This is the classic bug.
- Order matters: evict from the back *before* pushing, check expiry *before* emitting.
- `<` (not `<=`) when evicting keeps duplicates; `<=` collapses equal values onto the newer index -- both are correct.
- The first `k - 1` iterations only warm the deque; output starts at `i == k - 1`.

## Alternative -- max-heap per window

- A max-heap of `(value, index)` per window, O(n log k) total, with lazy deletion of expired indices.
- Simpler to reason about but strictly slower than the deque here. Pick it when you also need fast max updates between windows.

## Alternative -- segment tree / sparse table

- Range-max on *arbitrary* ranges after O(n log n) build -- overkill when every range has the same length k.
- Wins only when the kata also asks for arbitrary window sizes or point updates between windows.

## Complexity

- Time: O(n) amortized.
- Space: O(k) for the deque.

## Usage

- Real-time dashboards: max price, temperature, or throughput over the last k samples.
- Network packet analysis, rate-limit windows, "largest value per k-block" as a preprocessing step.
- The same trick answers sliding-window *minimum* -- flip the comparison; one algorithm gives both bounds.
- Any "what is the most extreme value over the last k events" query on a live stream.

## Cousins & contrasts

- **Daily temperatures**: the same monotonic idea with a stack -- resolves "next greater" distances for all positions, no moving window.
- **Monotonic queue**: the data-structure idea itself -- a deque where insertion order is the value order, not the insertion order.
- **Max-heap per window**: O(log k) per step plus lazy deletion of expired elements -- strictly worse here.
- **Prefix maximum array**: works for prefix windows (always starting at 0) but cannot shrink the start; the deque handles arbitrary windows.
