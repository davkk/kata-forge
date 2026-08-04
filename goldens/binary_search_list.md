# Binary Search -- one comparison kills half: O(log n) search on sorted data

## Core idea

- Invariant: *if the answer exists, it stays inside `[lo, hi)`* -- every step halves the window and never drops the answer.
- The test is a **monotone predicate** ("is `a[mid]` too small?"), not equality -- that is what makes half-kills safe.

## Build up

1. **Probe the middle**

```
int mid = lo + (hi - lo) / 2;
if (a[mid] == needle) return true;
```

2. **Too small -> needle is to the right**

```
if (a[mid] < needle) lo = mid + 1;   // kill everything <= mid
```

3. **Too big -> needle is to the left**

```
else hi = mid;                       // keep left half, [lo, hi)
```

4. **Repeat until the window is empty**

```
while (lo < hi) { ... }              // lo == hi -> not found
```

## Diagram

```
a = [ 1 3 5 7 9 11 13 ], needle = 9

step 1   lo       mid       hi      a[3]=7 < 9  ->  kill left half
        [ 1 3 5 7 9 11 13 )
                 ^
step 2            lo   mid  hi      a[5]=11 > 9 ->  kill right half
        [ 1 3 5 7 9 11 13 )
                      ^
step 3            lo   hi           a[4]=9 == 9  ->  found
        [ 1 3 5 7 9 11 13 )
                      ^
```

## Approach -- presence check

```cpp
using namespace std;

bool search(const vector<int>& a, int needle) {
    int lo = 0, hi = (int)a.size();        // [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;      // step 1
        if (a[mid] == needle) return true;
        if (a[mid] < needle) lo = mid + 1; // step 2
        else                 hi = mid;     // step 3
    }
    return false;                          // step 4
}
```

- `mid = lo + (hi - lo) / 2` avoids overflow (`(lo+hi)/2` is UB on huge indices).
- The same loop minus the equality return is `std::lower_bound` -- first index with `a[i] >= target`.

### Trace

- needle 9 in `[1,3,5,7,9,11,13]`: mid=3 (7<9 -> lo=4); mid=5 (11>9 -> hi=5); mid=4 -> true. 3 comparisons vs 7 linear.

## Complexity

- Time: O(log n). Space: O(1).

## Alternative -- binary search on the answer

- When feasibility is monotone ("smallest k that works"), run the same loop over the answer range `[min, max]` instead of array indices.

## Use when

- Sorted lookup, lower/upper bound, count occurrences, rotated arrays (see find_min_rotated_sorted), sorted 2D matrices, integer square root.
- Reach for this when the search space is sorted *or* has a monotone feasibility predicate -- the answer space itself can be searched.

## Cousins

- **BST**: the same halving as a data structure; O(log n) only while balanced.
- **Two pointers**: also shrinks a window, but from both ends -- for pair predicates.
- **Linear search**: O(n) and works unsorted; sortedness is the price of O(log n).
