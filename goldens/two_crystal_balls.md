# Two Crystal Balls

Find the break floor -- the first `1` in a **monotone** 0/1 array (`0...0 1...1`) -- while minimizing the worst-case number of drops with only 2 balls to break.

## Intuition

- Ball 1 jumps in steps of sqrt(n); once it breaks, the floor lies inside the last sqrt(n)-wide block, and ball 2 linear-scans that block.
- Balance the costs: jump size s costs <= n/s jumps plus <= s scans -- minimized at s = sqrt(n), giving worst case 2*sqrt(n) = O(sqrt(n)).
- Why not binary search: a break is irreversible -- halving needs a fresh ball per probe, i.e. log n balls. With two, you may "spend" exactly one break.
- Monotonicity is the whole game: all `false` then all `true`, so exactly one threshold exists.

## Approach -- sqrt(n) jumps + linear scan

```cpp
using namespace std;

int two_crystal_balls(const vector<int>& a) {
    int n = (int)a.size();
    int jump = max(1, (int)sqrt((double)n));

    int i = 0;
    while (i < n && !a[i]) i += jump;
    i = max(0, i - jump);

    while (i < n && !a[i]) ++i;
    return i < n ? i : -1;
}
```

- Rewind invariant: `i - jump` was checked safe (or we never moved), so the threshold is in `(i - jump, i]`.
- Clamp `jump` to >= 1 -- for tiny arrays `sqrt` truncates to 0, which would loop forever.
- Worst case is a break at the top of the last block: n/sqrt(n) jumps plus a full sqrt(n) scan.

## Alternative -- k balls (general)

- With k balls, jump by n^((k-1)/k) and drop one recursion level per broken ball -> worst case O(k * n^(1/k)).
- k = log2(n) balls recovers binary search: enough budget to halve every time.
- The k-ball generalisation is the "why is two-ball the standard puzzle" answer.

## Alternative -- exponential search (unbounded floors)

- Doubling jumps: 1, 2, 4, 8, ... until a break, then binary-search the last interval. O(log n) drops.
- Different cost model (probes are free, no balls to break), same coarse-then-fine shape.

## Complexity

- Time: O(sqrt(n)) worst case.
- Space: O(1).

## Usage

- Threshold finding under irreversible failure: drop tests, stress tests, any probe that destroys the specimen.
- **sqrt(n) as a technique**: the same jump-then-scan balance powers Mo's algorithm (offline range queries), sqrt decomposition / bucketing, and jump pointers.
- Any "find the breaking point with k shots" puzzle.

## Cousins & contrasts

- **Binary search**: O(log n) but assumes unlimited re-probing -- here each break is final.
- **Jump search**: the identical sqrt(n) stepping for sorted arrays where probes are free; the balls are the "expensive probe" twist.
- **Exponential search**: doubling jumps for unbounded arrays -- different cost model, same coarse-then-fine shape.
- **Interpolation search**: O(log log n) average on uniformly distributed sorted data -- assumes nothing about the irreversibility of probes.
