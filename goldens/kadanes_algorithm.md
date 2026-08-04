# Kadane's Algorithm -- best subarray ends somewhere: reuse the run ending at i - 1

## Core idea

- The best subarray ending at i reuses the best subarray ending at i - 1: dp[i] = max(a[i], dp[i-1] + a[i]) -- extend the old run or restart fresh at i.
- The global answer is the max over all ending points; since dp[i] reads only dp[i-1], two scalars replace the whole table.

## Build up

1. **Naive rescans every window**
```
for every (l, r): sum a[l..r] -> O(n^2)
```
2. **Ending at i suffices**
```
cur = best sum of a subarray that ends at i
```
3. **Extend or restart**
```
cur = max(a[i], cur + a[i])    // restart when cur is negative
```

## Diagram

```
a = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
i     0   1   2   3   4  5  6   7  8
cur  -2   1  -2   4   3  5  6   1  5    extend or restart
best -2   1   1   4   4  5  6   6  6    <- answer 6
[4, -1, 2, 1] = 6; cur below zero -> restart
```

## Approach -- single pass, two scalars

```cpp
using namespace std;

int max_subarray(const vector<int>& a) {
    int best = a[0], cur = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
        cur  = max(a[i], cur + a[i]);    // step 3: extend or restart
        best = max(best, cur);           // step 2: answer = max ending point
    }
    return best;
}
```

- `max(a[i], cur + a[i])` is the restart decision: a negative cur only drags a[i] down.
- Seed with a[0], never 0: all-negative input must report the true negative, not 0.

### Trace

- a = [-2,1,-3,4,-1,2,1,-5,4]: cur runs -2,1,-2,4,3,5,6,1,5; best peaks at 6 (indices 3..6).

## Complexity

- Time: O(n). Space: O(1).

## Alternative -- prefix sums

- Max subarray = max over j of prefix[j] - min prefix before j; same answer, but prefix sums also serve range-sum queries.

## Use when

- Reach for this when the problem asks the best sum of a contiguous window and a negative prefix can be thrown away -> one-pass DP.
- Max profit from price changes, largest gain/loss window, stock with cooldown.
- Any O(n^2) subarray brute force that should be O(n).

## Cousins

- **Maximum product subarray**: same shape, track max and min because negatives flip signs.
- **Sliding window**: fixed-size windows; Kadane's window is free-size, driven by the sign.
- **Prefix sums**: better when range-sum queries follow.
