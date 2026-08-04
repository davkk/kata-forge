# Two Sum Sorted -- the squeeze: two pointers bracket the target in O(n)

## Core idea

- Invariant: if a valid pair exists, it stays inside [l, r]; l holds the smallest and r the largest remaining candidate, so their sum brackets the target.
- Mechanism: a[l] + a[r] < k means a[l] is too small even for the largest partner, so l is safely retired; a sum too big retires r symmetrically.

## Build up

1. **Two pointers bracket the target**

```
int l = 0, r = (int)a.size() - 1;
while (l < r) { ... }
```

2. **Too small: advance l**

```
if (sum < k) ++l;
```

3. **Too big: retreat r**

```
else --r;
```

## Diagram

```
a = [2 7 11 15], k = 9

 l         r          2 + 15 = 17 > 9  ->  drop r
[2 7 11 15]
 l       r            2 + 11 = 13 > 9  ->  drop r
[2 7 11 15]
 l     r              2 + 7  = 9  == 9 ->  found {1, 2}
[2 7 11 15]
        arrows only move inward; each step retires one index
```

## Approach -- two pointers

```cpp
using namespace std;

vector<int> two_sum_sorted(const vector<int>& a, int k) {
    int l = 0, r = (int)a.size() - 1;   // step 1
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == k) return {l + 1, r + 1};
        if (sum < k) ++l;               // step 2
        else         --r;               // step 3
    }
    return {-1, -1};
}
```

- The if/else pair IS the discard rule from Core idea: one comparison decides which partner can never match.
- `{l + 1, r + 1}` because the contract is 1-indexed; `{-1, -1}` is the no-pair sentinel. The moves depend entirely on sortedness.

### Trace

- [2,7,11,15], k=9: 2+15=17 -> r--; 2+11=13 -> r--; 2+7=9 -> {1,2}. No pair: pointers cross -> {-1,-1}.

## Complexity

- Time: O(n), each index visited at most once. Space: O(1).

## Alternative -- hash map (unsorted input)

- Store value to index in a map while walking, check for k - a[i] on the fly: O(n) time and O(n) space, works unsorted, the general two-sum form.

## Use when

- Sorted input plus a pair or triplet condition: pair sums, price matching, net-zero offsets.
- Reach for this when "find two values summing to k" or as the inner squeeze of 3-sum and 4-sum.

## Cousins

- Two sum unsorted: the hash map variant, see alternative.
- Binary search per element: for each a[i], search k - a[i] in O(log n), total O(n log n), beaten by the squeeze.
- 3-sum: sort, fix one element, run this squeeze on the remainder.
- Sliding window: also two indices, but both move right for window predicates.
