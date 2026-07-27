# Knuth-Morris-Pratt (KMP) String Search

Find the first occurrence of a pattern in a text in O(n + m) -- the text pointer **never moves back**.

## Intuition

- Naive search restarts the pattern from scratch after every mismatch, wasting everything already matched. KMP's insight: the pattern's **self-overlap** tells you how much of the match survives a mismatch.
- Precompute the failure function: `lps[i]` = length of the longest proper prefix of `pattern[0..i]` that is also a suffix.
- On mismatch with j chars matched, fall back to `j = lps[j-1]` instead of `j = 0`: the longest prefix-suffix is guaranteed to still match the text behind you, so `i` keeps moving forward.
- O(m) build lps + O(n) scan = O(n + m) total, O(m) extra space.

## Approach -- build lps + scan

```cpp
using namespace std;

static vector<int> build_lps(const string& pat) {
    vector<int> lps(pat.size(), 0);
    int len = 0;
    for (int i = 1; i < (int)pat.size(); ) {
        if (pat[i] == pat[len]) lps[i++] = ++len;
        else if (len > 0)       len = lps[len - 1];  // fall back, retry same i
        else                    lps[i++] = 0;
    }
    return lps;
}

int kmp(const string& text, const string& pattern) {
    if (pattern.empty()) return 0;
    vector<int> lps = build_lps(pattern);
    int j = 0;
    for (int i = 0; i < (int)text.size(); ) {
        if (text[i] == pattern[j]) {
            ++i; ++j;
            if (j == (int)pattern.size()) return i - j;   // full match
        } else if (j > 0) j = lps[j - 1];   // reuse surviving prefix
        else              ++i;               // nothing matched: advance text
    }
    return -1;
}
```

### Walkthrough

Search `pattern = "aabaab"` in `text = "aabaabaab"`:
- Build lps = [0,1,0,1,2,3] (each entry is the longest prefix that is also a suffix for the prefix ending at that index)
- Scan: i=0..5 all match -> j=6, full match at i=6 -> return i - j = 0
- (A more interesting trace: pattern = "aab" in text = "aacaab")
  - lps = [0,1,0]
  - i=0 (a), j=0: match, j=1
  - i=1 (a), j=1: match, j=2
  - i=2 (c), j=2: mismatch; j = lps[1] = 1
  - i=2 (c), j=1: mismatch; j = lps[0] = 0
  - i=2 (c), j=0: mismatch; j==0 -> i=3
  - i=3 (a), j=0: match, j=1
  - i=4 (a), j=1: match, j=2
  - i=5 (b), j=2: match, j=3 -> full match at i=3

- The lps build is a mini-KMP: the pattern matches against its own prefixes with the same fall-back trick. Getting this right is the hard half.
- The fallback loops are amortized -- j shrinks only as many times as it grew, keeping the scan linear.
- Trace lps on `"aaab"` -> `[0,1,2,0]`; index-math bugs hide here, not in the scan.

## Complexity

- Time: O(n + m).
- Space: O(m) for the lps array.

## Alternative -- Z-algorithm (often simpler)

- Compute `z[i]` = length of the longest match between `s` and `s[i..]` in O(|s|). Concatenate `pattern + '#' + text`, run Z, and check `z[i] == |pattern|`.
- Same O(n + m), often shorter and easier to code than KMP.
- Slightly more auxiliary space (one Z array on the concatenated string) and an extra character that must not appear in either input.

## Alternative -- Rabin-Karp (rolling hash, many patterns)

- Hash the pattern; slide a same-sized window over the text, updating the hash in O(1) per step.
- O(n + m) average but O(n*m) worst case on hash collisions. Wins when you search many patterns at once (hash each, compare hashes).

## Alternative -- Boyer-Moore (sublinear in practice)

- Compare right-to-left; on a mismatch jump past the bad character and the bad suffix.
- Sublinear on large alphabets with sparse matches; the practical choice for `grep`-style search.

## Usage

- Searching fixed patterns in huge streams: log scanning, DNA motif search, `strstr`-grade library code.
- String problems with self-overlap: shortest palindrome by prefix, repeated-substring detection (lps of the string itself).
- Editor search, plagiarism detection, command-line tools like `grep` and `ctrl-F`.

## Cousins & contrasts

- **Rabin-Karp**: rolling hash, O(n+m) average -- wins for many-patterns-at-once, loses worst case to collisions.
- **Z-algorithm**: computes `z[i]` = match length at every position; simpler to code, same O(n+m), often interchangeable.
- **Boyer-Moore**: compares right-to-left and jumps on bad characters -- sublinear in practice, go-to for large alphabets.
- **Naive scan**: O(n*m), zero setup -- fine for short texts; baseline KMP's lps machinery replaces.
