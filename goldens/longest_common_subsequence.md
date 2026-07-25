# Longest Common Subsequence

Length of the longest subsequence common to two strings — same order, **not necessarily contiguous**. The two-string DP that a whole family of problems is a variant of.

## Intuition

- DP recipe: define state, write recurrence, set base cases, choose iteration order.
- State: `dp[i][j]` = LCS length of prefixes `a[0..i)` and `b[0..j)`. Row/col 0 are the empty-prefix base cases (0).
- Recurrence: if `a[i-1] == b[j-1]`, extend a common subsequence: `dp[i-1][j-1] + 1`. On a mismatch, drop one char from either side: `max(dp[i-1][j], dp[i][j-1])`.
- Why "drop from either" is safe: on a mismatch the two last chars can't both be used, so at least one is absent from any common subsequence — discarding `a`'s last *or* `b`'s last never kills the optimum, and `max` covers both cases.
- Fill row by row; each cell reads up, left, and up-left. O(m·n) time and space.

## Full table

```cpp
using namespace std;

int lcs(const string& a, const string& b) {
    int m = (int)a.size(), n = (int)b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    return dp[m][n];
}
```

- Mind the 1-index shift: `dp[i][j]` talks about `a[i-1]` / `b[j-1]` — the extra row/col makes empty prefixes free instead of special-cased.
- Answer is `dp[m][n]`, not the max over the table.

## Alternative: two rows + path reconstruction

- Row `i` reads only row `i - 1` — two rolling rows of size n+1 give the length in O(n) space.
- To recover the actual subsequence, walk the full table from `(m, n)`: on a match take the char and go up-left; on a mismatch move toward the larger neighbor. Needs the full table (or checkpointed), so it doesn't combine with the rolling-row trick.
- Hirschberg's algorithm recovers the string in O(n) space via divide & conquer, if you need both.

## Where it shows up

- Diff tools (`git diff` with lines as characters), file synchronization.
- DNA/protein sequence alignment in bioinformatics, plagiarism detection.
- Skeleton for the two-string DP family: edit distance, shortest common supersequence, longest repeated subsequence.

## Cousins & contrasts

- **Edit distance**: same table shape, different recurrence — mismatch costs `1 + min` of insert/delete/substitute instead of max-of-drops.
- **Longest common substring**: contiguity required → reset to 0 on mismatch, answer is the max cell, not `dp[m][n]`.
- **LIS**: with distinct elements, LCS of a sequence against its sorted self gives the longest increasing subsequence.
- **Longest palindromic subsequence**: LCS of `s` against `reverse(s)`.
