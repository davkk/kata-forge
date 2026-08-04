# Longest Common Subsequence -- compare last chars: match extends, mismatch drops

## Core idea

- The LCS of prefixes a[0..i) and b[0..j) reuses smaller prefixes: a match gives 1 + dp[i-1][j-1], a mismatch gives max(dp[i-1][j], dp[i][j-1]).
- On mismatch at least one of the two last chars is absent from any common subsequence, so dropping a's last or b's last covers every candidate.

## Build up

1. **Compare last characters**
```
if (a[i-1] == b[j-1]) a matched pair
```
2. **Match extends both prefixes**
```
dp[i][j] = dp[i-1][j-1] + 1
```
3. **Mismatch drops one side**
```
dp[i][j] = max(dp[i-1][j], dp[i][j-1])
```

## Diagram

```
a = "ABC", b = "AC"
        ""   A   C
""      0   0   0
A       0   1   1      match -> 1 + up-left
B       0   1   1      mismatch -> max(up, left)
C       0   1   2      match -> 2 ("AC")
cell reads up, left, up-left
```

## Approach -- full 2D table

```cpp
using namespace std;

int lcs(const string& a, const string& b) {
    int m = (int)a.size(), n = (int)b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            if (a[i - 1] == b[j - 1])               // step 1
                dp[i][j] = dp[i - 1][j - 1] + 1;    // step 2
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]); // step 3
    return dp[m][n];
}
```

- The 1-index shift makes empty prefixes cost 0 with no special cases.
- The answer is dp[m][n], not the table maximum.

### Trace

- a = "ABC", b = "AC": dp = [0,0,0; 0,1,1; 0,1,1; 0,1,2] -> return 2 ("AC").

## Complexity

- Time: O(m x n). Space: O(m x n); two rolling rows drop it to O(min(m, n)).

## Alternative -- two rolling rows

- Row i reads only row i - 1: keep prev and cur rows and swap, same recurrence, O(n) space for the length.

## Use when

- Reach for this when the problem asks the longest common ordered subset of two sequences -> prefix DP.
- Diff tools, DNA alignment, plagiarism checks.
- Skeleton for edit distance, shortest common supersequence, longest palindromic subsequence.

## Cousins

- **Edit distance**: same table, mismatch costs 1 + min(insert, delete, substitute).
- **Longest common substring**: contiguity required -> reset to 0 on mismatch, answer is the max cell.
- **Longest increasing subsequence**: LCS of the array with its sorted self.
