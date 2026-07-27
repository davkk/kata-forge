# Knuth-Morris-Pratt (KMP) String Search

Find the first occurrence of a pattern in a text in O(n + m) — the text pointer **never moves back**.

## Intuition

- Naive search restarts the pattern from scratch after every mismatch, wasting everything already matched. KMP's insight: the pattern's **self-overlap** tells you how much of the match survives a mismatch.
- Precompute the failure function: `lps[i]` = length of the longest proper prefix of `pattern[0..i]` that is also a suffix.
- On mismatch with j chars matched, fall back to `j = lps[j-1]` instead of `j = 0`: the longest prefix-suffix is guaranteed to still match the text behind you, so `i` keeps moving forward.
- Complexity: O(m) build lps + O(n) scan = O(n + m), O(m) space.

## Build lps + scan

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

- The lps build is a mini-KMP: the pattern matches against its own prefixes with the same fall-back trick. Getting this right is the hard half.
- The fallback loops are amortized — j shrinks only as many times as it grew, keeping the scan linear.
- Trace lps on `"aaab"` → `[0,1,2,0]`; index-math bugs hide here, not in the scan.

## Where it shows up

- Searching fixed patterns in huge streams: log scanning, DNA motif search, `strstr`-grade library code.
- String problems with self-overlap: shortest palindrome by prefix, repeated-substring detection (lps of the string itself).

## Cousins & contrasts

- **Rabin-Karp**: rolling hash, O(n+m) average — wins for many-patterns-at-once, loses worst case to collisions.
- **Z-algorithm**: computes `z[i]` = match length at every position; simpler to code, same O(n+m), often interchangeable.
- **Boyer-Moore**: compares right-to-left and jumps on bad characters — sublinear in practice, go-to for large alphabets.
- **Naive scan**: O(n·m), zero setup — fine for short texts; baseline KMP's lps machinery replaces.
