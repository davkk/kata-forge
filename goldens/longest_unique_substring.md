# Longest Unique Substring -- a window stays valid only while every char count is 1: grow right, heal left

## Core idea

- Invariant: [l, r] always holds a substring with all distinct chars; when s[r] repeats, shrink l until the duplicate is expelled.
- Why correct: every all-distinct substring is a window of this form, so the largest window length is the answer.

## Build up

1. **Try every start**
```
for each l: extend r while all distinct     // O(n^2) with a fresh set
```
2. **Grow the window**
```
freq[s[r]]++
```
3. **Heal the window**
```
while (freq[s[r]] > 1) { freq[s[l]]--; l++; }
```
4. **Measure**
```
best = max(best, r - l + 1)
```

## Diagram

```
s = "pwwkew"
r=0 'p'  freq p=1    l=0  best=1
r=1 'w'  freq w=1    l=0  best=2
r=2 'w'  freq w=2    l=2  best=2   drop p, w
r=3 'k'  freq k=1    l=2  best=2
r=4 'e'  freq e=1    l=2  best=3   "wke"
r=5 'w'  freq w=2    l=3  best=3   drop w -> "kew"
```

## Approach -- sliding window with frequency map

```cpp
using namespace std;

int longest_unique_substring(const string& s) {
    vector<int> freq(128, 0);
    int l = 0, best = 0;
    for (int r = 0; r < (int)s.size(); ++r) {
        freq[s[r]]++;                                  // step 2
        while (freq[s[r]] > 1) {                       // step 3
            freq[s[l]]--;
            l++;
        }
        best = max(best, r - l + 1);                   // step 4
    }
    return best;
}
```

- l and r only move forward: each char enters and leaves the window once, O(n) total.
- 128 covers ASCII; chars index the array directly.

### Trace

- s = "pwwkew": at r=2 the duplicate 'w' pushes l to 2; at r=5 the second 'w' pops the first, l=3; best stays 3 -> "wke" or "kew".

## Complexity

- Time: O(n). Space: O(1) (fixed 128 counters).

## Alternative -- last-seen jump

- Store each char's last position and jump l to max(l, last[s[r]] + 1) on a repeat: same O(n), no shrink loop.

## Use when

- Longest substring with no repeated char, or with at most k distinct chars, or any constraint on window contents.
- Streaming input: longest chunk without a duplicate token in a log or a stream.

## Cousins

- **Min window substring**: the same two-pointer frame with a target frequency to satisfy.
- **Longest substring with at most k distinct**: swap "all distinct" for a distinct-counter condition.
- **Permutation in string**: fixed-length window with a match counter.
