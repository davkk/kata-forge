# Coin Change

Minimum number of coins to make an amount, with unlimited coins of each denomination; coins are **reusable** so they are not part of the state. Return -1 if impossible.

## Intuition

- DP recipe: define state, write recurrence, set base cases, choose iteration order.
- State: `dp[a]` = fewest coins needed to make amount `a`. The state is over the amount axis only — coin supply is infinite, so which coins you've already used doesn't matter.
- Recurrence: `dp[a] = 1 + min_{c ≤ a} dp[a - c]` — for each coin denomination, treat it as the *last* coin used; the prefix `a - c` must itself be optimal (optimal substructure).
- Base case: `dp[0] = 0`; all other entries initialized to a sentinel > any possible answer.
- O(amount × #coins) time, O(amount) space.

## Bottom-up DP

```cpp
using namespace std;

int coin_change(int amount, const vector<int>& coins) {
    const int INF = amount + 1;              // sentinel > any real answer
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a)
        for (int c : coins)
            if (c <= a && dp[a - c] + 1 < dp[a])
                dp[a] = dp[a - c] + 1;
    return dp[amount] == INF ? -1 : dp[amount];
}
```

- Sentinel `amount + 1` avoids overflow from `INT_MAX + 1`; no solution needs more than `amount` coins (all 1s).
- **Loop-order pitfall**: outer amounts / inner coins computes the MIN (correct for min-coins). Swapping to outer coins / inner amounts counts *combinations* in the count-ways variant — same loop structure, different problem.
- Always guard `c <= a` before indexing `dp[a - c]`.

## Alternative: greedy (largest coin first)

- Repeatedly take the biggest coin ≤ remaining amount; O(n log n) after sorting.
- Correct only for **canonical** coin systems (US 1/5/10/25). Fails on {1,3,4}, amount 6: greedy picks 4+1+1 (3 coins), optimal is 3+3 (2).
- Fine as a heuristic or when canonicity is proven; DP is the safe default.

## Where it shows up

- Vending-machine change, currency arbitrage, postage-stamp problems.
- Template for unbounded "fewest parts" problems: min perfect squares summing to n, min cuts to reach a length.
- The count-ways twin ("how many ways make the amount") has the same table structure with `+` instead of `min` — and the combination-loop order.

## Cousins & contrasts

- **Unbounded knapsack**: same "items reusable" structure; coin change minimizes count for an exact sum, knapsack maximizes value under a capacity.
- **0/1 knapsack**: each item at most once → items enter the state (or loop direction flips).
- **BFS on the amount graph**: amounts as nodes, coins as edges — correct but strictly more work than DP.
- **Greedy**: O(n log n), only for canonical systems — {1,3,4} counterexample above.
