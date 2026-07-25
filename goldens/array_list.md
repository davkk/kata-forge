# Array List

A dynamic array: O(1) indexed access plus O(1) **amortized** append, paid for by doubling capacity whenever the backing array fills up.

## Intuition

- Keep three pieces of state: `data` (raw array), `len` (elements in use), `cap` (allocated slots). `get(i)` is a bounds check + `data[i]`.
- Append writes at `data[len]`; when `len == cap`, allocate `2*cap`, copy everything over, free the old array.
- Why doubling makes append O(1) amortized: copies happen at sizes 1, 2, 4, …, n — total copy work is 1+2+…+n ≈ 2n spread over n appends, ~2 copies each on average. Growing by a *constant* (e.g. +16) instead gives O(n) amortized.
- Insert/remove in the middle shifts the tail left or right — O(n). That shift is the price of contiguous storage.
- Space O(cap) ≤ 2·O(n): a just-grown array is at most half empty.

## Implementation

```cpp
using namespace std;

struct ArrayList {
    int* data;
    int  len;
    int  cap;
};

void grow(ArrayList& l) {
    l.cap *= 2;
    int* next = new int[l.cap];
    for (int i = 0; i < l.len; ++i) next[i] = l.data[i];
    delete[] l.data;
    l.data = next;
}

void append(ArrayList& l, int x) {
    if (l.len == l.cap) grow(l);
    l.data[l.len++] = x;
}

void insertAt(ArrayList& l, int x, int idx) {
    if (l.len == l.cap) grow(l);
    for (int i = l.len; i > idx; --i) l.data[i] = l.data[i - 1];  // shift right
    l.data[idx] = x;
    l.len++;
}

int removeAt(ArrayList& l, int idx) {
    int val = l.data[idx];
    for (int i = idx; i < l.len - 1; ++i) l.data[i] = l.data[i + 1];  // shift left
    l.len--;
    return val;
}
```

- `prepend(x)` is `insertAt(l, x, 0)`; `remove(x)` is a linear scan then `removeAt`. Don't duplicate the shifting logic.
- Grow *before* writing, not after — writing into a full array is the classic heap overflow.
- Shift direction matters: shift-right walks `i` from `len` *down* to `idx`, shift-left walks *up*. Flip it and you overwrite the element you still need.

## Alternative: linked list

- Prepend/insert/remove become O(1) pointer rewiring once you're at the position, but `get(i)` degrades to O(n) and each element costs a pointer + a heap allocation.
- Pick the array when indexed access dominates; pick the list when middle-of-sequence edits dominate.

## Where it shows up

- `std::vector`, Python `list`, Java `ArrayList` — the default sequential container in most languages.
- Backing store for stacks, binary heaps, hash-table buckets, adjacency arrays.
- Amortized doubling is *the* canonical example for amortized analysis.

## Cousins & contrasts

- **std::vector**: the production version — same growth policy plus iterators and RAII. Its `size()` vs `capacity()` is exactly `len` vs `cap` here.
- **Static array**: fixed capacity, no growth — simpler, but overflow is failure instead of reallocation.
- **Linked list**: O(1) splicing at a known position, O(n) random access — the opposite trade-off.
