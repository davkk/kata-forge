# 0/1 Knapsack

Max total value fitting in a weight capacity, **each item taken at most once**. The canonical subset-selection DP -- pseudo-polynomial in the capacity.

## Intuition

- State: `dp[w]` = best value achievable with capacity `w` using items considered so far. Compressed from the full 2D `dp[i][w]` = best using the first `i` items.
- Recurrence per item: `dp[w] = max(dp[w], dp[w - wi] + vi)` -- either skip the item or take it and add its value to the best packing of the remaining capacity.
- Optimal substructure: in any optimal packing, item `i` is either absent (the optimum without it) or present (`vi` plus the optimum on `w - wi` without it).
- Base case: all zeros -- empty capacity or no items give value 0.

## Approach 1 -- 1D DP, capacity iterated BACKWARDS

```cpp
using namespace std;

int knapsack(int capacity, const vector<int>& weights, const vector<int>& values) {
    vector<int> dp(capacity + 1, 0);
    for (int i = 0; i < (int)weights.size(); ++i)
        for (int w = capacity; w >= weights[i]; --w)
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
    return dp[capacity];
}
```

- This compresses the 2D origin: `dp[i][w]` reads only row `i - 1`, so one rolling row suffices.
- **Backwards `w` is the whole point**: descending means `dp[w - wi]` still holds the *previous* item's row, so item `i` enters at most once. Ascending re-reads cells already updated by item `i` and the same item gets taken repeatedly -- accidental unbounded knapsack.

## Approach 2 -- top-down recursion with memoization

```cpp
using namespace std;

int knapsack(int capacity, const vector<int>& weights, const vector<int>& values) {
    int n = (int)weights.size();
    vector<vector<int>> memo(n + 1, vector<int>(capacity + 1, -1));
    function<int(int,int)> go = [&](int i, int w) -> int {
        if (i == 0 || w == 0) return 0;
        if (memo[i][w] != -1) return memo[i][w];
        int skip = go(i - 1, w);
        int take = (weights[i-1] <= w)
                 ? values[i-1] + go(i - 1, w - weights[i-1])
                 : 0;
        return memo[i][w] = max(skip, take);
    };
    return go(n, capacity);
}
```

- Reads as a direct translation of the recurrence: take item `i` (adding its value to the optimum on the remaining capacity) or skip it.
- Stack depth O(n) plus memo table O(n * capacity); the iterative form is leaner when the capacity is large but `n` is small.

## Alternative -- meet-in-the-middle (large n, small capacity)

- Split items into two halves of size n/2; enumerate all 2^(n/2) subsets of each, recording `(weight, value)` pairs.
- Sort one half by weight, drop dominated pairs (higher weight, lower value), then for each pair in the other half binary-search the best compatible value.
- Brings the runtime down from O(n * capacity) to O(2^(n/2) * n) -- exponential in n, but n can be ~40 instead of ~20.

## Complexity

- Time: O(n * capacity).
- Space: O(capacity) for the 1D version; O(n * capacity) for the 2D recursive form.

## Usage

- Resource allocation: budgets, cargo loading, portfolio selection, picking tasks under a time limit.
- Subset-sum and partition-equal-subset are knapsacks with value == weight.
- Base for approximation schemes: the general problem is NP-hard, but this DP is exact when the capacity is modest.
- Decisions over "should I include this item" are everywhere -- knapsack is the simplest version that captures them all.

## Cousins & contrasts

- **Unbounded knapsack**: items reusable -> same 1D code with the inner loop ascending. One direction flip changes the problem.
- **Subset-sum**: boolean version -- "is sum T achievable?" -- same table, OR instead of max.
- **Partition equal subset sum**: subset-sum with target = total / 2.
- **Fractional knapsack**: items divisible -> greedy by value/weight ratio, no DP needed. The 0/1 constraint is what forces DP.
- **Coin change**: unbounded structure, minimizing count for an exact sum rather than maximizing value under a capacity.
