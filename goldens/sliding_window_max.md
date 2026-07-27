# Sliding Window Maximum

Maximum of every contiguous window of size k, in O(n) total — a **monotonic deque** instead of rescanning each window (O(n·k)).

## Intuition

- Maintain a deque of **indices** whose values are strictly decreasing; the front is always the current window's maximum.
- When a new element arrives, pop from the back everything smaller than it: those can *never* be the max again — the newcomer is larger and outlives them in every future window.
- Pop from the front any index that has left the window (`<= i - k`).
- Each index is pushed once and popped at most once → amortized O(1) per step, O(n) total; the deque holds at most k entries → O(k) space.
- Mental model: the deque is the list of *candidates* for future maxima, decreasing in value and increasing in age.

## Monotonic deque

```cpp
using namespace std;

vector<int> max_sliding_window(const vector<int>& a, int k) {
    vector<int> out;
    deque<int> dq;                              // indices, values decreasing
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] < a[i])
            dq.pop_back();                      // beaten forever by a[i]
        dq.push_back(i);
        if (dq.front() <= i - k)                // expired: outside window
            dq.pop_front();
        if (i >= k - 1)                         // window full → emit max
            out.push_back(a[dq.front()]);
    }
    return out;
}
```

- Store **indices, not values** — window expiry (`i - k`) can't be checked from values alone. This is the classic bug.
- Order matters: evict from the back *before* pushing, check expiry *before* emitting.
- `<` (not `<=`) when evicting keeps duplicates; `<=` collapses equal values onto the newer index — both are correct.
- The first `k - 1` iterations only warm the deque; output starts at `i == k - 1`.

## Where it shows up

- Real-time dashboards: max price / temperature / throughput over the last k samples.
- Network packet analysis, rate-limit windows, "largest value per k-block" as a preprocessing step.
- Same trick answers sliding-window *minimum* — flip the comparison; one algorithm gives both bounds.

## Cousins & contrasts

- **daily_temperatures**: the same monotonic idea with a stack — resolves "next greater" distances for all positions, no moving window.
- **Segment tree / sparse table**: range-max on *arbitrary* ranges after O(n log n) build — overkill when every range has the same length k.
- **Max-heap per window**: O(log k) per step plus lazy deletion of expired elements — the deque is strictly better here.
