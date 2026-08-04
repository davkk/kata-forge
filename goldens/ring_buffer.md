# Ring buffer -- modulo wraps the array: bounded FIFO without shifting or allocation

## Core idea

- Invariant: `count` live elements start at `buf[head]` and wrap modulo `cap`; the physical array never moves.
- Mechanism: every index advances as `(i + 1) % cap`, so "next slot" is arithmetic -- old slots get overwritten in place.

## Build up

1. **Tail is derived, not stored**

```
int tail = (head + count) % cap;
```

2. **Push: write there, count it**

```
buf[tail] = x;
count++;
```

3. **Full: clobber the oldest**

```
if (count == cap) head = (head + 1) % cap;   // overwrite policy
```

4. **Pop: read head, advance**

```
int v = buf[head];
head = (head + 1) % cap;
count--;
```

5. **Get: index from the head**

```
return buf[(head + i) % cap];
```

## Diagram

```
cap 3                        push 4 (full):        push 5:
buf [ 1 2 3 ]                buf [ 4 2 3 ]         buf [ 4 5 3 ]
     h                           h                      h
count 3                     tail=(0+3)%3=0         tail=(1+3)%3=1
                            write over oldest 1    head->2

pop -> 3, then 4, then 5: order preserved by the wrap
```

## Approach -- count field, overwrite oldest

```cpp
using namespace std;

class RingBuffer {
public:
    int cap;
    vector<int> buf;
    int head;
    int count;

    RingBuffer(int cap) : cap(cap), buf(cap), head(0), count(0) {}

    void push(int x) {
        int tail = (head + count) % cap;    // step 1
        if (count == cap)
            head = (head + 1) % cap;        // step 3
        else
            count++;
        buf[tail] = x;                      // step 2
    }

    optional<int> pop() {
        if (count == 0) return nullopt;
        int val = buf[head];                // step 4
        head = (head + 1) % cap;
        count--;
        return val;
    }

    optional<int> get(int i) {
        if (i < 0 || i >= count) return nullopt;
        return buf[(head + i) % cap];       // step 5
    }

    int size() { return count; }
};
```

- `head + count` derives the tail (step 1) -- one fewer index to keep in sync.
- `count` disambiguates full from empty (head == tail in both states); when full, push advances head after computing tail, so it overwrites the oldest slot.

### Trace

- cap 3: push 1, 2, 3 fills it; push 4 wraps tail=(0+3)%3=0 and moves head to 1, clobbering slot 0; push 5 -> tail 1, head 2; pop -> 3, then 4, then 5.

## Complexity

- Time: O(1) per push/pop/get. Space: O(cap), fixed.

## Alternative -- sacrifice one slot

- Drop `count`; empty is `tail == head`, full is `(tail + 1) % cap == head`; wastes a slot, but two indices alone enable lock-free single-producer/single-consumer use.

## Use when

- Reach for this when you need a bounded FIFO that never shifts or allocates: "keep the last N", streaming.
- Producer/consumer, audio jitter buffers, NIC rings, flight recorders.

## Cousins

- **Queue**: unbounded FIFO that grows (see queue).
- **Array list**: contiguous and growable; the ring trades growth for wrap.
- **Circular linked list**: the same wrap with pointers, worse cache behavior.
- **Deque**: a growable ring with both ends open.
