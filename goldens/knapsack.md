# 0/1 Knapsack -- max value in a capacity: dp[w] reuses dp[w - wi], swept backwards

## Core idea

- The best packing for capacity w reuses the best packing for capacity w - wi: taking item i adds vi to the optimum on the leftover, skipping keeps the old value, so dp[w] = max(dp[w], vi + dp[w - wi]).
- 0/1 means each item enters at most once: iterate capacities BACKWARDS so dp[w - wi] still holds the previous item's row.

## Build up

1. **Try every subset**
```
for each subset: sum weights, sum values -> 2^n, hopeless
```
2. **Best value per capacity**
```
dp[w] = best value achievable with capacity w so far
```
3. **Iterate capacities backwards**
```
for w = capacity down to weights[i]    // 0/1, one use per item
```

## Diagram

```
weights [2,3,4], values [3,4,5], cap 5
w:       0  1  2  3  4  5
start    0  0  0  0  0  0
item 0   0  0  3  3  3  3
item 1   0  0  3  4  4  7
item 2   0  0  3  4  5  7   <- max(7, dp[1]+5=5): skip
backwards keeps rows apart; ascending re-takes the item
```

## Approach -- 1D, capacity backwards

```cpp
using namespace std;

int knapsack(int capacity, const vector<int>& weights, const vector<int>& values) {
    vector<int> dp(capacity + 1, 0);
    for (int i = 0; i < (int)weights.size(); ++i)        // step 1: one item at a time
        for (int w = capacity; w >= weights[i]; --w)     // step 3: BACKWARDS
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]); // step 2: take or skip
    return dp[capacity];
}
```

- Backwards is the whole point: descending w keeps dp[w - wi] from the previous item, so item i is used at most once.
- Ascending w would re-read just-updated cells and take the same item repeatedly.

### Trace

- weights [2,3,4], values [3,4,5], cap 5: dp runs [0,0,3,3,3,3], [0,0,3,4,4,7], [0,0,3,4,5,7] -> return 7 (items 0+1).

## Complexity

- Time: O(n x capacity). Space: O(capacity).

## Alternative -- top-down memoization

- Recursive go(i, w) = max(skip, take) with a 2D memo; reads exactly like the recurrence, same time, more stack.

## Use when

- Reach for this when the problem asks max/min over a subset that fits a capacity budget, each item at most once -> 0/1 knapsack DP.
- Budgets, cargo loading, tasks under a time limit.
- Subset-sum and partition-equal-subset: the same table with value == weight.

## Cousins

- **Coin change**: unbounded (ascending loop), minimizes count for an exact sum.
- **Unbounded knapsack**: one direction flip changes the whole problem.
- **Fractional knapsack**: items divisible -> greedy by value/weight, no DP.
