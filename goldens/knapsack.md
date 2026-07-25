# 0/1 Knapsack

Max total value fitting in a weight capacity, **each item taken at most once**. The canonical subset-selection DP; pseudo-polynomial in the capacity.

## Intuition

- DP recipe: define state, write recurrence, set base cases, choose iteration order.
- State: `dp[w]` = best value achievable with capacity `w` using items considered so far. Compressed from the full 2D `dp[i][w]` = best using first `i` items.
- Recurrence per item: `dp[w] = max(dp[w], dp[w - wi] + vi)` — skip the item, or take it and add its value to optimal packing of the remaining capacity.
- Optimal substructure: in an optimal packing, item `i` is either absent (optimum without it) or present (`vi` + optimum on `w - wi` without it).
- Base case: all zeros — empty capacity or no items give value 0. O(n × capacity) time, O(capacity) space.

## 1D DP — capacity iterated BACKWARDS

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
- **Backwards `w` is the whole point**: descending means `dp[w - wi]` still holds the *previous* item's row, so item `i` enters at most once. Ascending re-reads cells already updated by item `i` → same item gets taken repeatedly → accidental unbounded knapsack.
- Stop at `w >= weights[i]`; below that `dp[w]` is unchanged.

## Where it shows up

- Resource allocation: budgets, cargo loading, portfolio selection, picking tasks under a time limit.
- Subset-sum and partition-equal-subset are knapsacks with value == weight.
- Base for approximation schemes: the general problem is NP-hard, but this DP is exact when capacity is modest.

## Cousins & contrasts

- **Unbounded knapsack**: items reusable → same 1D code with `w` ascending. One loop direction flips the problem.
- **Subset-sum**: boolean version — "is sum T achievable?" — same table, OR instead of max.
- **Partition equal subset sum**: subset-sum with target = total / 2.
- **Fractional knapsack**: items divisible → greedy by value/weight ratio, no DP needed. The 0/1 constraint is what forces DP.
- **Coin change**: unbounded structure, minimizing count for an exact sum rather than maximizing value under a capacity.
