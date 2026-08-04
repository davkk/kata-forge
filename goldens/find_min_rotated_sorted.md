# Find Min Rotated -- hunt the wrap point: one comparison halves the window

## Core idea

- Invariant: rotation splits the array into two sorted runs where everything left exceeds everything right; the min is the head of the right run, and the window keeps it inside [lo, hi].
- Mechanism: `a[mid] > a[hi]` proves mid sits in the big run -> the min is strictly right of mid; otherwise mid is in the small run -> the min is at or left of mid.

## Build up

1. **Probe the middle**

```
int mid = lo + (hi - lo) / 2;
```

2. **Mid in the big run: go right**

```
if (a[mid] > a[hi]) lo = mid + 1;
```

3. **Mid in the small run: go left**

```
else hi = mid;                 // keep mid: it may be the min
```

4. **Window shrinks to one**

```
while (lo < hi) { ... }        // lo == hi -> a[lo] is the min
```

## Diagram

```
a = [4 5 6 7 0 1 2], min = 0, the head of the small run

lo   mid        hi    a[3]=7 > a[6]=2 ->  min right of mid
[4 5 6 7 0 1 2]
      ^
   lo mid  hi          a[5]=1 <= a[6]=2 ->  min at or left of mid
[4 5 6 7 0 1 2]
        ^
   lo  hi              a[4]=0 <= a[5]=1 ->  hi = mid
[4 5 6 7 0 1 2]
     ^
   lo == hi -> return a[4] = 0
```

## Approach -- halve against the right end

```cpp
using namespace std;

int find_min_rotated(const vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {                    // step 4
        int mid = lo + (hi - lo) / 2;    // step 1
        if (a[mid] > a[hi]) lo = mid + 1; // step 2
        else                hi = mid;    // step 3
    }
    return a[lo];
}
```

- `hi = mid`, never `mid - 1`: mid itself may be the min; `lo = mid + 1` is safe only where mid provably is not.
- Already sorted input: `a[mid] <= a[hi]` always, so `hi` walks to 0, still O(log n). Duplicates break the halving (the `--hi` fix degrades to O(n)).

### Trace

- [4,5,6,7,0,1,2]: mid=3 (7 > 2 -> lo=4); mid=5 (1 <= 2 -> hi=5); mid=4 (0 <= 1 -> hi=4); lo == hi == 4 -> return 0.

## Complexity

- Time: O(log n) for distinct values, O(n) worst case with duplicates. Space: O(1).

## Alternative -- linear scan

- Track a running minimum in one pass: O(n), no rotation reasoning; use when sortedness is not guaranteed or n is tiny.

## Use when

- Rotated or circular sorted data: logs, circular buffers, "search in rotated array" as the pivot-finding first half.
- Reach for this when "sorted but shifted" and the answer is the wrap point.

## Cousins

- binary_search_list: the pure form, one monotone predicate, no rotation twist.
- Search in rotated array: this approach to find the pivot, then a binary search inside the right run.
- Find peak element: same halving, but the predicate compares a[mid] vs a[mid + 1].
