# Coin Change -- fewest coins for a sum: dp[n] reuses dp[n - c]

## Core idea

- The best answer for amount n reuses the best answer for amount n - c: dp[n] = 1 + min over coins c of dp[n - c] -- the last coin c finishes the amount, and the leftover is an independent smaller problem.
- Coins are unbounded, so only the amount matters: one table indexed by amount replaces an exponential subset search.

## Build up

1. **Greedy largest coin fails**
```
coins {1,3,4}, amount 6 -> greedy 4+1+1 (3), optimal 3+3 (2)
```
2. **Last coin finishes the amount**
```
dp[n] = 1 + min over coins c of dp[n - c]
```
3. **Iterate amounts upward**
```
for a in 1..amount: settle dp[a] from smaller cells
```

## Diagram

```
coins {1,3,4}, amount 6
n:   0  1  2  3  4  5  6
dp:  0  1  2  1  1  2  2
dp[6] = 1 + min(dp[5], dp[3], dp[2]) = 1 + min(2, 1, 2) = 2
every arrow reads a SMALLER cell -> fill left to right
```

## Approach -- bottom-up table

```cpp
using namespace std;

int coin_change(int amount, const vector<int>& coins) {
    const int INF = amount + 1;                    // sentinel > any real answer
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a)              // step 3: smaller amounts first
        for (int c : coins)                        // step 2: try each last coin
            if (c <= a && dp[a - c] + 1 < dp[a])
                dp[a] = dp[a - c] + 1;
    return dp[amount] == INF ? -1 : dp[amount];    // step 3: unreachable
}
```

- `INF = amount + 1`: even all-pennies uses at most `amount` coins, so INF safely means unreachable.
- The cell rule is the diagram verbatim: cheapest last coin plus 1.

### Trace

- coins {1,3,4}, amount 6: dp = [0,1,2,1,1,2,2]; dp[6] = 1 + min(2,1,2) = 2 -> return 2 (3+3, beating greedy).

## Complexity

- Time: O(amount x #coins). Space: O(amount).

## Alternative -- greedy

- Grab the largest coin that fits; correct only for canonical systems (US 1/5/10/25). DP is the safe default.

## Use when

- Reach for this when the problem asks fewest (or count of) parts summing to an exact total with reusable parts -> DP over amounts.
- Vending change, perfect squares, min cuts to reach length n.
- Count-ways twin: same table, sum instead of min.

## Cousins

- **Knapsack**: reusable items too, but maximizes value under a capacity (see knapsack).
- **BFS on amounts**: coins as edges between amounts; correct, heavier than DP.
- **LCS**: same "reuse smaller cells" table, over two string indices.
