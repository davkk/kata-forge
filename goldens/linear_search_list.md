# Linear Search

Scan the array front to back and return on the first match — O(n), with **no precondition**: its only virtue is that it works on any data.

## Intuition

- No structure to exploit ⇒ every element is a suspect; a miss costs exactly n probes, a hit n/2 on average.
- O(1) space, trivially correct — the baseline every smarter search must beat.
- For one-off queries on unsorted data it's optimal: sorting (Θ(n log n)) or building a hash set (Θ(n)) only pays off across many lookups.

## Scan

```cpp
using namespace std;

bool search(const vector<int>& haystack, int needle) {
    for (int x : haystack)
        if (x == needle) return true;
    return false;
}
```

- Early return on hit; reaching the end means absent.
- Nothing to get wrong — no indices, no bounds arithmetic.

## Alternative: sentinel search

- Overwrite the last slot with the needle (saving the old value), then loop `while (a[i] != needle) ++i;` — no per-iteration bounds check, roughly half the comparisons.
- Afterwards: a hit at `i < n - 1` is real; a hit at `n - 1` counts only if the saved value equals the needle.

## Where it shows up

- Unsorted data with few queries: contact lists, ad-hoc log grepping.
- Structures without random access: linked lists, streams.
- Hardware-assisted scans: `memchr`, vectorized `std::find` — linear search SIMD-izes beautifully.
- Small n, where binary search's bookkeeping costs more than the scan it saves.

## Cousins & contrasts

- **Binary search**: O(log n) but only on sorted data — sortedness is the price of admission.
- **Hash set**: O(1) average lookup after an O(n) build; wins when queries ≫ build cost.
- **Two pointers**: also linear, but exploits sortedness or pairing structure to prune candidates.
