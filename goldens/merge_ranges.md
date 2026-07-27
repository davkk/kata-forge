# Merge Ranges

Merge all overlapping `[start, end]` intervals into a minimal set of disjoint intervals. One sort plus one linear sweep.

## Intuition

- **Sort by start.** After that, intervals that can merge form a *contiguous run* in sorted order — a later interval can only join the run if its start ≤ the run's current end.
- Sweep once, keeping one open interval: if the next interval overlaps it, extend the open end; otherwise close it and open a new one.
- Why one pass suffices: sorting removes every "earlier start" case from consideration, so overlap becomes a purely local test against the current run's end.
- O(n log n) total — the sort dominates; the sweep itself is O(n). O(1) extra space beyond the output when sorting in place.

## Sort + sweep

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

- Extend with `max(end, iv[1])`, never plain `iv[1]` — a *contained* interval like `[2,3]` inside `[1,10]` would otherwise shrink the run.
- Empty input needs no special case: the `out.empty()` guard makes the first interval open a run, and an empty list returns empty.
- Touching intervals `[1,4] + [4,5]`: `<=` merges them, `<` keeps them apart — both conventions exist; read the problem statement.

## Where it shows up

- Calendar availability / free-busy computation, consolidating meeting bookings.
- IP-range consolidation, genome interval unions, version-range resolution.
- Preprocessing: merge first, then binary-search which run a query point falls into.

## Cousins & contrasts

- **Insert interval**: add one new interval to an already-sorted disjoint list — no sort needed, same extend-or-close logic in O(n).
- **Meeting rooms II** (minimum rooms): sort starts against ends or keep a min-heap of end times — it counts *concurrency* rather than unioning the ranges.
- **Interval scheduling** (max non-overlapping subset): greedy by *earliest end* — a selection problem, not a merge; the sort key changes because the objective changes.
