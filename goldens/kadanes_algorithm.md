# Kadane's Algorithm

Largest sum of any **contiguous** subarray, in one O(n) pass with O(1) space. The "ending at i" state turns a 2D-looking problem into a single running scalar.

## Intuition

- State: `dp[i]` = max sum of a subarray **ending exactly at `i`**. The "ending at `i`" constraint is what makes the recurrence trivial.
- Recurrence: `dp[i] = max(a[i], dp[i-1] + a[i])` -- extend the best run ending at `i-1`, or restart fresh at `i`.
- The global answer is the max over all `dp[i]`. Since `dp[i]` reads only `dp[i-1]`, two scalars replace the whole array.
- Optimal substructure: the best subarray ending at `i` is either just `a[i]`, or `a[i]` appended to the best subarray ending at `i-1`.
- Seed with `a[0]`, never 0 -- on all-negative input, 0-init silently reports 0 (an empty subarray) instead of the true negative answer.

## Approach -- single pass, two scalars

```cpp
using namespace std;

int max_subarray(const vector<int>& a) {
    int best = a[0], cur = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
        cur  = max(a[i], cur + a[i]);    // extend or restart
        best = max(best, cur);
    }
    return best;
}
```

### Walkthrough

On `a = [-2, 1, -3, 4, -1, 2, 1, -5, 4]`:
- i=0: best=-2, cur=-2
- i=1 (1): cur=max(1, -2+1)=1; best=1 (restart, since -2+1 < 1)
- i=2 (-3): cur=max(-3, 1-3)=-2; best=1
- i=3 (4): cur=max(4, -2+4)=4; best=4 (restart)
- i=4 (-1): cur=max(-1, 4-1)=3; best=4
- i=5 (2): cur=max(2, 3+2)=5; best=5
- i=6 (1): cur=max(1, 5+1)=6; best=6
- i=7 (-5): cur=max(-5, 6-5)=1; best=6
- i=8 (4): cur=max(4, 1+4)=5; best=6
- return 6 (subarray [4, -1, 2, 1] at indices 3-6)

- The `max(a[i], cur + a[i])` line *is* the restart decision: when `cur < 0`, adding `a[i]` only drags it down, so a fresh start at `a[i]` is better.
- `best` and `cur` are the only state -- no need to keep the whole `dp` array.

## Complexity

- Time: O(n).
- Space: O(1).

## Alternative -- return the subarray indices

- Track a `start` index, reset it to `i` whenever you restart, and snapshot `(start, i)` whenever `best` improves.

```cpp
int start = 0, lo = 0, hi = 0;
int best = a[0], cur = a[0];
for (int i = 1; i < n; ++i) {
    if (cur + a[i] < a[i]) { cur = a[i]; start = i; }
    else                    cur += a[i];
    if (cur > best) { best = cur; lo = start; hi = i; }
}
```

- The "what is the subarray, not just its sum" variant -- a few extra scalars, no asymptotic cost.
- Divide and conquer also works: max of left half, right half, and the best subarray crossing the middle. O(n log n) and strictly dominated by Kadane; only the crossing-sum pattern is worth remembering.

## Usage

- Max profit from a price-change series, largest gain/loss window in telemetry, signal processing.
- Gateway DP: the "ending at i" state generalizes to max product subarray, circular subarray max, stock-with-cooldown.
- The standard answer when an O(n^2) subarray brute force needs to become O(n).
- Any "best stretch of consecutive measurements" question in finance, biology, or performance monitoring.

## Cousins & contrasts

- **Prefix sums**: max subarray also equals `max(prefix[j] - min_prefix_before_j)`; Kadane is the idiomatic one-pass form, prefix sums win when you need range-sum *queries* instead.
- **Sliding window**: both pointers move right for fixed/constraint-size windows; Kadane's window is free-size and driven by the running sum's sign.
- **Maximum product subarray**: same "ending at i" shape, but a negative flips signs -> track both max and min ending at `i`.
- **Two pointers**: need a monotone predicate; subarray-sum-max with negatives isn't monotone, hence DP.
