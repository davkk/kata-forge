# Merge Ranges

Merge all overlapping `[start, end]` intervals into a minimal set of disjoint intervals. One sort plus one linear sweep.

## Intuition

- **Sort by start.** After that, intervals that can merge form a *contiguous run* in sorted order -- a later interval can only join the run if its start is <= the run's current end.
- Sweep once, keeping one open interval: if the next interval overlaps it, extend the open end; otherwise close it and open a new one.
- Why one pass suffices: sorting removes every "earlier start" case from consideration, so overlap becomes a purely local test against the current run's end.
- O(n log n) total -- the sort dominates; the sweep itself is O(n). O(1) extra space beyond the output when sorting in place.

## Approach -- sort + sweep

```cpp
using namespace std;

vector<pair<int,int>> merge_ranges(vector<pair<int,int>> a) {
    sort(a.begin(), a.end());                          // by start, then end
    vector<pair<int,int>> out;
    for (auto& iv : a) {
        if (!out.empty() && iv.first <= out.back().second)
            out.back().second = max(out.back().second, iv.second);
        else
            out.push_back(iv);
    }
    return out;
}
```

- Extend with `max(end, iv[1])`, never plain `iv[1]` -- a *contained* interval like `[2,3]` inside `[1,10]` would otherwise shrink the run.
- Empty input needs no special case: the `out.empty()` guard makes the first interval open a run, and an empty list returns empty.
- Touching intervals `[1,4] + [4,5]`: `<=` merges them, `<` keeps them apart -- both conventions exist; read the problem statement.

## Alternative -- sweep line with events (continuous intervals)

- Emit `(start, +1)` and `(end, -1)` events; sort and sweep, maintaining a depth counter. Overlapping run boundaries become "depth returns to zero" points.
- Same O(n log n), works on continuous or open/closed intervals without a discrete sort key.
- Best when intervals are open/closed or unbounded on one side.

## Alternative -- line sweep with interval tree (dynamic intervals)

- For a stream of insert and remove operations, keep an interval tree (or augmented BST) keyed by start.
- Insert O(log n), query "merge with neighbors" O(log n + k) for k merged intervals. Right choice when intervals are added one at a time, not known in advance.

## Complexity

- Time: O(n log n) -- the sort dominates; sweep is O(n).
- Space: O(n) for the output, O(1) extra when sorting in place.

## Usage

- Calendar availability / free-busy computation, consolidating meeting bookings.
- IP-range consolidation, genome interval unions, version-range resolution.
- Preprocessing: merge first, then binary-search which run a query point falls into.
- Any "combine these overlapping ranges into the minimum number of runs" task.

## Cousins & contrasts

- **Insert interval**: add one new interval to an already-sorted disjoint list -- no sort needed, same extend-or-close logic in O(n).
- **Minimum meeting rooms**: sort starts against ends or keep a min-heap of end times -- counts *concurrency* rather than unioning the ranges.
- **Interval scheduling** (max non-overlapping subset): greedy by *earliest end* -- a selection problem, not a merge; the sort key changes because the objective changes.
- **Union of intervals (measure)**: same sweep, but accumulate the total covered length instead of the run list.
