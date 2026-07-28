# Ring Buffer

Fixed-capacity FIFO over a circular array: head and tail advance and wrap modulo capacity. **Bounded memory with O(1) push/pop** makes it the standard producer/consumer buffer.

## Intuition

- `buf[head]` is the oldest element, `buf[tail]` the next free slot; both advance with `(i + 1) % cap` and wrap around the end -- that's the ring.
- Never shifts elements and never allocates: overwrite-in-place keeps every op O(1) and memory fixed at `cap` slots.
- **Full vs. empty ambiguity**: after wrapping, `head == tail` holds both when empty and when full. Fix it with a `count` field (or sacrifice one slot).
- Decide the full-policy up front: overwrite oldest / reject (streaming) vs. block / grow (lossless).

## Approach -- count field, overwrite oldest

```cpp
using namespace std;

struct RingBuffer {
    int cap;
    vector<int> buf;
    int head;
    int count;

    RingBuffer(int cap) : cap(cap), buf(cap), head(0), count(0) {}

    void push(int x) {
        int tail = (head + count) % cap;
        if (count == cap)
            head = (head + 1) % cap;
        else
            count++;
        buf[tail] = x;
    }

    optional<int> pop() {
        if (count == 0) return nullopt;
        int val = buf[head];
        head = (head + 1) % cap;
        count--;
        return val;
    }

    optional<int> get(int i) {
        if (i < 0 || i >= count) return nullopt;
        return buf[(head + i) % cap];
    }

    int size() { return count; }
};
```

- Derive `tail` from `head + count` instead of storing it -- one state variable fewer to desynchronize.
- Pitfall: every index must pass through `% cap` *after* the addition; `(head + idx)` unwrapped reads out of bounds once the window has passed the end.
- Pitfall: on full-with-overwrite, advance `head` -- the slot you're about to clobber is the oldest element. Skip it and order silently corrupts.

## Alternative -- sacrifice one slot

- Drop `count`; declare empty when `tail == head` and full when `(tail + 1) % cap == head`.
- Saves maintaining `count` but wastes one slot and forces you to store `tail` -- pick whichever state you'd rather not track.
- Common in low-level kernels where the savings matter.

## Alternative -- block on full / grow on full

- Replace the overwrite-oldest branch with `block` (sleep until consumer pops) or `grow` (reallocate a bigger buffer and copy).
- Lossless for the block version, unbounded growth for the grow version. The producer/consumer API looks the same; only the back-pressure policy changes.

## Complexity

- Time: O(1) per push and pop.
- Space: O(cap), fixed.

## Usage

- Producer/consumer between threads or interrupt handlers: fixed latency, no allocation, no unbounded growth.
- Audio/video streaming and jitter buffers: overwriting the oldest sample beats blocking.
- Logging and flight-recorder buffers: keep the last N events in bounded memory (circular logs in embedded systems).
- OS/network internals: NIC DMA rings, keyboard buffers, pipe implementations.

## Cousins & contrasts

- **Growable queue**: same FIFO order but grows instead of rejecting/overwriting -- choose it when losing data is unacceptable.
- **Dynamic array**: also contiguous, but grows by copying; the ring trades growth for wrap-around reuse of the same slots.
- **Circular linked list**: same wrap idea with pointers; the array version wins on cache locality and zero allocation.
