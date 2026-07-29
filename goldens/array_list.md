# Array List

A dynamic array: O(1) indexed access plus O(1) **amortized** append, paid for by doubling capacity whenever the backing array fills up.

## Intuition

- Keep three pieces of state: `data` (raw array), `len` (elements in use), `cap` (allocated slots). `get(i)` is a bounds check plus `data[i]`.
- Append writes at `data[len]`; when `len == cap`, allocate `2*cap`, copy everything over, free the old array.
- Why doubling makes append O(1) amortized: copies happen at sizes 1, 2, 4, ..., n -- total copy work is 1+2+...+n ~= 2n spread over n appends, ~2 copies each on average. Growing by a *constant* (e.g. +16) instead gives O(n) amortized.
- Insert/remove in the middle shifts the tail left or right -- O(n). That shift is the price of contiguous storage.
- Space O(cap) <= 2*O(n): a just-grown array is at most half empty.

## Approach -- growth + shifting

```cpp
using namespace std;

struct ArrayList {
    int cap;
    int* data;
    int len;

    ArrayList(int cap) : cap(cap), data(new int[cap]), len(0) {}

    void append(int x) {
        if (len == cap) grow();
        data[len++] = x;
    }

    void prepend(int x) { insertAt(0, x); }

    void insertAt(int i, int x) {
        if (len == cap) grow();
        for (int j = len; j > i; --j) data[j] = data[j - 1];
        data[i] = x;
        len++;
    }

    optional<int> removeAt(int i) {
        if (i < 0 || i >= len) return nullopt;
        int val = data[i];
        for (int j = i; j < len - 1; ++j) data[j] = data[j + 1];
        len--;
        return val;
    }

    optional<int> remove(int x) {
        for (int i = 0; i < len; ++i) {
            if (data[i] == x) return removeAt(i);
        }
        return nullopt;
    }

    optional<int> get(int i) {
        if (i < 0 || i >= len) return nullopt;
        return data[i];
    }

    int size() { return len; }

    void grow() {
        cap *= 2;
        int* next = new int[cap];
        for (int i = 0; i < len; ++i) next[i] = data[i];
        delete[] data;
        data = next;
    }
};
```

- `prepend(x)` is `insertAt(0, x)`; `remove(x)` is a linear scan then `removeAt`. Don't duplicate the shifting logic.
- Grow *before* writing, not after -- writing into a full array is the classic heap overflow.
- Shift direction matters: shift-right walks `j` from `len` *down* to `i`, shift-left walks *up*. Flip it and you overwrite the element you still need.

## Alternative -- linked list

- Prepend/insert/remove become O(1) pointer rewiring once you're at the position, but `get(i)` degrades to O(n) and each element costs a pointer + a heap allocation.
- Pick the array when indexed access dominates; pick the list when middle-of-sequence edits dominate.

## Alternative -- circular buffer for fixed capacity

- Same O(1) access and O(1) append, but never grows; overwrites the oldest on full. See ring_buffer.
- The right choice when the workload is naturally bounded and growth is the exception.

## Complexity

- Time: O(1) amortized append, O(1) get, O(n) insert/remove, O(n) search. Space: O(n), with at most 2x waste.

## Usage

- `std::vector`, Python `list`, Java `ArrayList` -- the default sequential container in most languages.
- Backing store for stacks, binary heaps, hash-table buckets, adjacency arrays.
- Amortized doubling is *the* canonical example for amortized analysis.

## Cousins & contrasts

- **`std::vector`**: the production version -- same growth policy plus iterators and RAII. Its `size()` vs `capacity()` is exactly `len` vs `cap` here.
- **Static array**: fixed capacity, no growth -- simpler, but overflow is failure instead of reallocation.
- **Linked list**: O(1) splicing at a known position, O(n) random access -- the opposite trade-off.
- **Ring buffer**: same O(1) but with a fixed cap; the array list trades the cap guarantee for growth.
