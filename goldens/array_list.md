# Array list -- doubling spreads the copy: amortized O(1) append in contiguous memory

## Core idea

- Invariant: the first `len` slots of `data` hold the live elements in order; `len <= cap` always.
- Mechanism: growth doubles `cap`, so total copy work 1+2+4+...+n ~ 2n spreads over n appends -- constant on average.

## Build up

1. **Write at the next slot**

```
data[len++] = x;
```

2. **Guard: grow when full**

```
if (len == cap) grow();
```

3. **Double, copy, swap**

```
cap *= 2;
int* next = new int[cap];
for (int i = 0; i < len; i++) next[i] = data[i];
delete[] data; data = next;
```

4. **Middle insert shifts right**

```
for (int j = len; j > i; j--) data[j] = data[j - 1];
data[i] = x; len++;
```

5. **Middle remove shifts left**

```
for (int j = i; j < len - 1; j++) data[j] = data[j + 1];
len--;
```

## Diagram

```
cap 3                          append 11 (full -> grow to 6)
[ 5 7 9 ]                      [ 5 7 9 11 _ _ ]      copy 3, cap 6
 len=3 cap=3

insertAt(1, 6):                removeAt(1) -> 7:
[ 5 6 7 9 11 _ ]               [ 5 7 9 11 _ _ ]
      shift right                   shift left
```

## Approach -- growth + shifting

```cpp
using namespace std;

class ArrayList {
public:
    int cap;
    int len;
    int* data;

    ArrayList(int cap) : cap(cap), data(new int[cap]), len(0) {}

    void append(int x) {
        if (len == cap) grow();     // step 2
        data[len++] = x;            // step 1
    }

    void prepend(int x) { insertAt(0, x); }

    void insertAt(int i, int x) {
        if (len == cap) grow();
        for (int j = len; j > i; --j) data[j] = data[j - 1]; // step 4
        data[i] = x;
        len++;
    }

    optional<int> removeAt(int i) {
        if (i < 0 || i >= len) return nullopt;
        int val = data[i];
        for (int j = i; j < len - 1; ++j) data[j] = data[j + 1]; // step 5
        len--;
        return val;
    }

    optional<int> remove(int x) {
        for (int i = 0; i < len; ++i)
            if (data[i] == x) return removeAt(i);
        return nullopt;
    }

    optional<int> get(int i) {
        if (i < 0 || i >= len) return nullopt;
        return data[i];
    }

    int size() { return len; }

    void grow() {                   // step 3
        cap *= 2;
        int* next = new int[cap];
        for (int i = 0; i < len; ++i) next[i] = data[i];
        delete[] data;
        data = next;
    }
};
```

- `append` is step 1 behind step 2's guard; `grow` is step 3 -- doubling (not `cap + k`) is what makes the copy total geometric.
- `insertAt`/`removeAt` are steps 4-5, the contiguity tax; `prepend` and `remove` compose those primitives.

### Trace

- ArrayList(3): append 5, 7, 9 fills cap; append 11 grows to 6 (copy 3, one O(n) payment); insertAt(1, 6) shifts 7, 9 right; get(2) -> 7 straight off `data[2]`.

## Complexity

- Time: O(1) amortized append, O(1) get, O(n) insert/remove/remove-by-value. Space: O(n), <= 2x waste.

## Alternative -- linked list

- O(1) splice once you hold the node, O(n) random access -- the inverse trade (see doubly_linked_list).

## Use when

- Reach for this when you need random access plus unbounded append -- vector, Python list, ArrayList.
- Pattern trigger: a rare O(n) step whose cost scales geometrically (double/halve) can be charged to a run of cheap steps -- the canonical amortized O(1) proof.
- Backing store for stacks, heaps, hash buckets, adjacency arrays.

## Cousins

- **Static array**: fixed cap; overflow is a bug, not a grow.
- **Linked list**: O(1) splice, O(n) index -- the opposite trade.
- **Ring buffer**: fixed cap, wraps instead of growing.
- **std::vector**: the same policy plus RAII, iterators, and `size()` vs `capacity()`.
