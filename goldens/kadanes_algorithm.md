# Kadane's Algorithm

Largest sum of any **contiguous** subarray, in one O(n) pass with O(1) space.

## Intuition

- DP view: `dp[i]` = max sum of a subarray **ending exactly at `i`**. The "ending at `i`" constraint makes the recurrence trivial — extend the best run ending at `i-1`, or restart fresh at `i`.
- `dp[i] = max(a[i], dp[i-1] + a[i])`: extending is worth it unless the running sum has gone negative, in which case it only drags you down — restart is better exactly then.
- The global answer is `max` over all `dp[i]`; since `dp[i]` reads only `dp[i-1]`, two scalars replace the array.
- Optimal substructure: the best subarray ending at `i` either is just `a[i]`, or appends `a[i]` to the best subarray ending at `i-1`.
- O(n) time, O(1) space.

## Single pass

```cpp
using namespace std;

int max_subarray(const vector<int>& nums) {
    int best = nums[0], cur = nums[0];        // seed with a[0], NOT 0
    for (int i = 1; i < (int)nums.size(); ++i) {
        cur  = max(nums[i], cur + nums[i]);   // extend or restart
        best = max(best, cur);
    }
    return best;
}
```

- **Seed with `nums[0]`, never 0**: on all-negative arrays, 0-init silently reports 0 (empty subarray) instead of the true (negative) answer. If empty subarrays are allowed, 0-init is correct.
- No explicit restart branch needed: `max(nums[i], cur + nums[i])` *is* "restart iff cur < 0".

## Alternative: return the subarray indices

- Track a `start` index; reset it to `i` whenever you restart, and snapshot `(start, i)` whenever `best` improves.

```cpp
int start = 0, lo = 0, hi = 0;
for (int i = 1; i < n; ++i) {
    if (cur + nums[i] < nums[i]) { cur = nums[i]; start = i; }
    else                          cur += nums[i];
    if (cur > best) { best = cur; lo = start; hi = i; }
}
```

- Divide & conquer also exists (max of left half, right half, best crossing the middle), O(n log n) — but strictly dominated by Kadane; only the crossing-sum pattern is worth remembering.

## Where it shows up

- Max profit from a price-change series, largest gain/loss window in telemetry, signal processing.
- Gateway DP: the "ending at i" state generalizes to max product subarray, circular subarray max, stock-with-cooldown.
- The standard answer when an O(n²) subarray brute force needs to become O(n).

## Cousins & contrasts

- **Prefix sums**: max subarray also equals `max(prefix[j] - min_prefix_before_j)`; Kadane is the idiomatic one-pass form, prefix sums win when you need range-sum *queries* instead.
- **Sliding window**: both pointers move right for fixed/constraint-size windows; Kadane's window is free-size and driven by the running sum's sign.
- **Maximum product subarray**: same "ending at i" shape, but a negative flips signs → track both max and min ending at `i`.
- **Two pointers**: need a monotone predicate; subarray-sum-max with negatives isn't monotone, hence DP.
