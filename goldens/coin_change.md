# Coin Change

Minimum number of coins to make an amount, with unlimited coins of each denomination. Coins are **reusable**, so which coins have already been used does not matter. Return -1 if impossible.

## Intuition

- Classic unbounded DP: define a state over the **amount axis only**, because coin supply is infinite.
- State: `dp[a]` = fewest coins needed to make amount `a`.
- Recurrence: `dp[a] = 1 + min over c <= a of dp[a - c]` -- treat each denomination as the *last* coin used; the prefix must itself be optimal (optimal substructure).
- Base case: `dp[0] = 0`; every other entry starts at a sentinel larger than any real answer.
- The loop order outer-amount / inner-coin computes the **min**; swapping the loops counts **combinations** instead -- same table, different problem.

## Approach 1 -- bottom-up DP (iterative)

```cpp
using namespace std;

int coin_change(int amount, const vector<int>& coins) {
    const int INF = amount + 1;                    // sentinel > any real answer
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a)
        for (int c : coins)
            if (c <= a && dp[a - c] + 1 < dp[a])
                dp[a] = dp[a - c] + 1;
    return dp[amount] == INF ? -1 : dp[amount];
}
```

- Sentinel `amount + 1` beats `INT_MAX` to dodge `+1` overflow; no solution ever needs more than `amount` coins (all 1s).
- Always guard `c <= a` before indexing `dp[a - c]`.

## Approach 2 -- top-down recursion with memoization

```cpp
using namespace std;

int coin_change(int amount, const vector<int>& coins) {
    vector<int> memo(amount + 1, -2);             // -2 = uncomputed
    function<int(int)> go = [&](int a) -> int {
        if (a == 0) return 0;
        if (memo[a] != -2) return memo[a];
        int best = INT_MAX;
        for (int c : coins)
            if (c <= a) best = min(best, go(a - c));
        return memo[a] = (best == INT_MAX ? -1 : 1 + best);
    };
    return go(amount);
}
```

- Mirrors the recurrence directly: `coin_change(a) = 1 + min(coin_change(a - c))`.
- Easier to reason about when the recurrence has tricky edge cases; the iterative form avoids stack depth on large amounts.

## Alternative -- greedy (largest coin first)

- Repeatedly take the biggest coin <= remaining amount; O(n log n) after sorting.
- Correct only for **canonical** coin systems (US 1/5/10/25). Fails on {1, 3, 4} at amount 6: greedy picks 4+1+1 (3 coins), optimal is 3+3 (2).
- Fine as a fast heuristic when canonicity is proven; DP is the safe default.

## Complexity

- Time: O(amount * #coins).
- Space: O(amount).

## Usage

- Vending-machine change, currency arbitrage, postage-stamp problems.
- Template for any unbounded "fewest parts" problem: min perfect squares summing to n, min cuts to reach a length.
- The count-ways twin ("how many ways to make the amount") has the same table with `+` instead of `min` and the combination-loop order.

## Cousins & contrasts

- **Unbounded knapsack**: same "items reusable" structure; coin change minimizes count for an exact sum, knapsack maximizes value under a capacity.
- **0/1 knapsack**: each item at most once -- items enter the state and the inner loop direction flips.
- **BFS on the amount graph**: amounts as nodes, coins as edges -- correct but strictly more work than DP.
- **Greedy**: O(n log n), only for canonical systems -- {1, 3, 4} is the classic counterexample.
