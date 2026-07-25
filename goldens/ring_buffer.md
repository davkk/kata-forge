# Ring Buffer

Fixed-capacity FIFO over a circular array: head and tail advance and wrap modulo capacity. **Bounded memory with O(1) push/pop** makes it the standard producer–consumer buffer.

## Intuition

- `buf[head]` is the oldest element, `buf[tail]` the next free slot; both advance with `(i + 1) % cap` and wrap around the end — that's the ring.
- Never shifts elements and never allocates: overwrite-in-place keeps every op O(1) and memory fixed at `cap` slots.
- **Full vs. empty ambiguity**: after wrapping, `head == tail` holds both when empty and when full. Fix it with a `count` field (or sacrifice one slot).
- Decide the full-policy up front: overwrite oldest / reject (streaming) vs. block / grow (lossless).

## Implementation (count field, overwrite oldest)

```cpp
using namespace std;

struct Ring {
    int* buf;
    int  head;     // oldest element
    int  count;    // elements stored — kills the full/empty ambiguity
    int  cap;
};

void push(Ring& r, int x) {
    int tail = (r.head + r.count) % r.cap;
    if (r.count == r.cap)            // full: tail == head, oldest slot dies
        r.head = (r.head + 1) % r.cap;
    else
        r.count++;
    r.buf[tail] = x;
}

int pop(Ring& r) {                   // caller checks r.count > 0
    int val = r.buf[r.head];
    r.head = (r.head + 1) % r.cap;
    r.count--;
    return val;
}

int get(Ring& r, int idx) {          // idx counted from head, not from 0
    return r.buf[(r.head + idx) % r.cap];
}
```

- Derive `tail` from `head + count` instead of storing it — one state variable fewer to desynchronize.
- Pitfall: every index must pass through `% cap` *after* the addition; `(head + idx)` unwrapped reads out of bounds once the window has passed the end.
- Pitfall: on full-with-overwrite, advance `head` — the slot you're about to clobber is the oldest element. Skip it and order silently corrupts.

## Alternative: sacrifice one slot

- Drop `count`; declare empty when `tail == head` and full when `(tail + 1) % cap == head`.
- Saves maintaining `count` but wastes one slot and forces you to store `tail` — pick whichever state you'd rather not track.

## Where it shows up

- Producer–consumer between threads or interrupt handlers: fixed latency, no allocation, no unbounded growth.
- Audio/video streaming and jitter buffers: overwriting the oldest sample beats blocking.
- Logging and flight-recorder buffers: keep the last N events in bounded memory (circular logs in embedded systems).
- OS/network internals: NIC DMA rings, keyboard buffers, pipe implementations.

## Cousins & contrasts

- **Growable queue**: same FIFO order but grows instead of rejecting/overwriting — choose it when losing data is unacceptable.
- **Dynamic array**: also contiguous, but grows by copying; the ring trades growth for wrap-around reuse of the same slots.
- **Circular linked list**: same wrap idea with pointers; the array version wins on cache locality and zero allocation.
