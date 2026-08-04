# Minimum Window Substring -- one counter tracks missing target chars; shrink while nothing is missing

## Core idea

- Invariant: missing counts target chars not yet satisfied inside [l, r]; the window is valid exactly when missing == 0.
- Why correct: every valid window is measured the moment it becomes valid, and shrinking keeps the shortest for each right edge, so the global minimum is found.

## Build up

1. **Count the target**
```
for (c in t) need[c]++
```
2. **Grow the window**
```
have[s[r]]++; if (have[c] <= need[c]) missing--
```
3. **Shrink while valid**
```
while (missing == 0) { record length; drop s[l]; if (have[d] < need[d]) missing++; l++; }
```
4. **Sentinel**
```
bestLen == s.size() + 1  ->  return ""
```

## Diagram

```
s = "abca", t = "ba"
r=0 'a'  have a=1   missing 1
r=1 'b'  have b=1   missing 0   -> win "ab" (2)
r=2 'c'  have c=1   missing 1
r=3 'a'  have a=1   missing 0   -> win "bca" (3) too long, shrink past
```

## Approach -- two-pointer with a match counter

```cpp
using namespace std;

string min_window_substring(const string& s, const string& t) {
    vector<int> need(128, 0), have(128, 0);
    for (char c : t) need[c]++;
    int missing = (int)t.size();
    int l = 0, bestL = 0, bestLen = (int)s.size() + 1;
    for (int r = 0; r < (int)s.size(); ++r) {
        char c = s[r];
        have[c]++;
        if (have[c] <= need[c]) missing--;             // step 2
        while (missing == 0) {                         // step 3
            if (r - l + 1 < bestLen) {
                bestLen = r - l + 1;
                bestL = l;
            }
            char d = s[l];
            have[d]--;
            if (have[d] < need[d]) missing++;
            l++;
        }
    }
    if (bestLen == (int)s.size() + 1) return "";       // step 4
    return s.substr(bestL, bestLen);
}
```

- A char counts as satisfied once its have count meets need; extra copies never touch missing.
- bestLen is s.size() + 1 so no window ever beats the sentinel; the check doubles as the "no window" answer.

### Trace

- s = "ADOBECODEBANC", t = "ABC": first valid window "ADOBEC" (6) at r=5; later windows are longer, until r=12 "C" re-satisfies the last char and shrinking from "DEBANC" (6) slides l to 9 -> "BANC" (4); nothing shorter follows.

## Complexity

- Time: O(n). Space: O(1) (fixed 128 counters).

## Alternative -- reuse longest-unique frame

- Same structure as longest_unique_substring; the only difference is the validity test (missing == 0 instead of freq <= 1) and recording instead of measuring.

## Use when

- Shortest substring containing all chars of a target: anagram searches, substring with all characters of a pattern.
- Generalize the counters to any multiset condition on window contents.

## Cousins

- **Longest unique substring**: the same two-pointer frame, valid when no char repeats.
- **Permutation in string**: fixed-size window over s that must equal t's frequency table.
- **Find all anagrams**: record every valid fixed-size window instead of the shortest.
