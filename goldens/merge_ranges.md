# Merge Ranges -- sort by start, then one sweep: overlap is a local test

## Core idea

- After sorting by start, overlapping intervals form a contiguous run: a later interval joins the run iff next.start <= last.end.
- Keep one open run; extend its end with max(end, next.end), close it and open a new run when the test fails.

## Build up

1. **Sort by start**
```
sort(a.begin(), a.end());
```
2. **Overlap test**
```
if (next.first <= last.second)    touch or cross
```
3. **Extend with max(end)**
```
last.second = max(last.second, next.second);
```

## Diagram

```
sorted: [1,3] [2,6] [8,10] [15,18]
[1,3] -> [1,6] -> [1,6] + [8,10] + [15,18]
2 <= 3 extends; 8 > 6 commits; 15 > 10 commits
contained [2,3] inside [1,10]: max(10, 3) = 10, no shrink
```

## Approach -- sort + sweep

```cpp
using namespace std;

vector<pair<int,int>> merge_ranges(vector<pair<int,int>> a) {
    sort(a.begin(), a.end());                          // step 1: by start, then end
    vector<pair<int,int>> out;
    for (auto& iv : a) {
        if (!out.empty() && iv.first <= out.back().second)    // step 2: overlap test
            out.back().second = max(out.back().second, iv.second); // step 3: extend
        else
            out.push_back(iv);                          // step 3: open a new run
    }
    return out;
}
```

- `<=` merges touching intervals; `<` keeps [1,4] and [4,5] apart -- both conventions exist, read the statement.
- The max (not bare iv.second) keeps a contained interval from shrinking the run.

### Trace

- [[1,3],[2,6],[8,10],[15,18]]: open [1,3]; [2,6] extends to [1,6]; 8 > 6 and 15 > 10 open fresh runs -> out = [[1,6],[8,10],[15,18]].

## Complexity

- Time: O(n log n), the sort dominates. Space: O(n) for the output.

## Alternative -- sweep line events

- Emit (start, +1) and (end, -1), merge wherever depth returns to zero; same O(n log n), handles open/closed endpoints.

## Use when

- Reach for this when overlapping or touching intervals must be unioned into a minimal disjoint set -> sort by start, then sweep.
- Calendar availability, IP consolidation, genome interval unions.
- Preprocess first, then binary-search which run a query point falls into.

## Cousins

- **Insert interval**: one new interval into a sorted disjoint list, no sort, O(n).
- **Minimum meeting rooms**: counts concurrency with a heap of end times, not a union.
- **Interval scheduling**: max non-overlapping subset by earliest end -- selection, not merge.
