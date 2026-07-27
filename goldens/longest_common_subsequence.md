# Longest Common Subsequence

Length of the longest subsequence common to two strings -- same relative order, **not necessarily contiguous**. The two-string DP that a whole family of problems is built on.

## Intuition

- State: `dp[i][j]` = LCS length of the prefixes `a[0..i)` and `b[0..j)`. The extra row and column make empty prefixes cost 0 without special cases.
- Recurrence: if `a[i-1] == b[j-1]`, extend a common subsequence: `dp[i-1][j-1] + 1`. On a mismatch, drop the last char from either side: `max(dp[i-1][j], dp[i][j-1])`.
- The mismatch rule is safe: at least one of the two last chars is absent from any common subsequence, so discarding `a`'s last *or* `b`'s last cannot kill the optimum, and `max` covers both.
- Fill row by row; each cell reads up, left, and up-left.

## Approach 1 -- full 2D table

```cpp
using namespace std;

int longest_common_subsequence(const string& a, const string& b) {
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

### Walkthrough

On `a = "ABCBDAB"`, `b = "BDCAB"` (m=7, n=5):
- Build the table column by column; mismatches take `max` of left and up
- `dp[1][1]` (A,B): mismatch -> max(0,0)=0
- `dp[1][2]` (A,D): mismatch -> 0; `dp[2][1]` (B,B): match -> dp[1][1]+1=1
- `dp[4][4]` (B,A): mismatch -> max(dp[3][4]=2, dp[4][3]=1)=2
- `dp[7][5]` (B,B): match -> dp[6][4]+1=4
- return 4 (the subsequence "BCAB" or "BDAB")

A short prefix (a="ABC", b="AC"):
- dp:
  ```
     "" A C
  ""  0 0 0
  A   0 1 1
  B   0 1 1
  C   0 1 2
  ```
- return 2 ("AC")

- The 1-index shift is intentional: `dp[i][j]` talks about `a[i-1]` and `b[j-1]`, so empty prefixes become free.
- The answer is `dp[m][n]`, not the maximum over the table.

## Complexity

- Time: O(m*n).
- Space: O(m*n) for the 2D table; O(min(m, n)) if only the length is needed (rolling rows).

## Approach 2 -- top-down recursion with memoization

```cpp
using namespace std;

int longest_common_subsequence(const string& a, const string& b) {
    int m = (int)a.size(), n = (int)b.size();
    vector<vector<int>> memo(m + 1, vector<int>(n + 1, -1));
    function<int(int,int)> go = [&](int i, int j) -> int {
        if (i == 0 || j == 0) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        if (a[i-1] == b[j-1]) return memo[i][j] = 1 + go(i-1, j-1);
        return memo[i][j] = max(go(i-1, j), go(i, j-1));
    };
    return go(m, n);
}
```

- Mirrors the recurrence directly: same branching on match / mismatch, memoized to keep the work at O(m*n).
- More readable for irregular cases; the bottom-up form is leaner for the simple length-only problem.

## Alternative -- two rolling rows + path reconstruction

- Row `i` reads only row `i - 1`, so two rolling rows of size n+1 give the length in O(n) space.
- To recover the actual subsequence, walk the full table from `(m, n)`: on a match take the char and go up-left, on a mismatch move toward the larger neighbor. Needs the full table (or checkpointed), so it does not combine with the rolling-row trick.
- Hirschberg's algorithm recovers the string in O(n) space via divide and conquer, if both matter.

## Usage

- Diff tools (`git diff` with lines as characters) and file synchronization.
- DNA and protein sequence alignment in bioinformatics, plagiarism detection.
- Skeleton for the entire two-string DP family: edit distance, shortest common supersequence, longest repeated subsequence.
- Any "how much do these two sequences have in common" question, from version control to speech recognition.

## Cousins & contrasts

- **Edit distance**: same table shape, different recurrence -- mismatch costs `1 + min` of insert/delete/substitute instead of `max`-of-drops.
- **Longest common substring**: contiguity required -> reset to 0 on mismatch, answer is the max cell, not `dp[m][n]`.
- **Longest increasing subsequence**: with distinct elements, LCS of a sequence against its sorted self.
- **Longest palindromic subsequence**: LCS of `s` against `reverse(s)`.
