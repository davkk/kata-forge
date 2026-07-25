# Daily Temperatures

For each day, how many days until a warmer temperature (0 if none). Solved in O(n) with a **monotonic stack** of unresolved days.

## Intuition

- Keep a stack of indices whose temperatures are **decreasing** — days still waiting for a warmer day.
- When day `i` is warmer than the stack top, it is *the* answer for that top: pop and record `i - idx`. One warm day can resolve a whole run of colder pending days.
- Then push `i` itself: it now waits for its own warmer day.
- Amortized O(n): each index is pushed once and popped at most once → at most 2n stack operations. Space O(n) for the stack.
- Why it works: the decreasing invariant means the stack top is always the *nearest unresolved day that `i` can resolve* — deeper entries are colder still and must keep waiting.

## Monotonic stack

```cpp
using namespace std;

vector<int> daily_temperatures(const vector<int>& t) {
    vector<int> out(t.size(), 0);               // 0 = no warmer day ever
    vector<int> st;                             // stack of indices, temps decreasing
    for (int i = 0; i < (int)t.size(); ++i) {
        while (!st.empty() && t[st.back()] < t[i]) {
            int idx = st.back(); st.pop_back();
            out[idx] = i - idx;                 // i is idx's first warmer day
        }
        st.push_back(i);
    }
    return out;
}
```

- Push **indices, not temperatures** — the answer is a distance `i - idx`, not a value.
- Strict `<` in the pop condition: equal temperatures don't resolve (the question asks for *warmer*, not "at least as warm").
- Indices left on the stack never see a warmer day — their 0 defaults are already correct, so no cleanup pass is needed.

## Contrast: brute force

- Naive: from each day, scan right until something warmer appears — O(n²) worst case (strictly decreasing input never resolves early).
- The stack is the same search with memory: every "known colder" day is parked by position and resolved in O(1) when its answer arrives.

## Where it shows up

- The entire "next greater element" family: stock span, next-higher price, signal-peak distances.
- Weather/climate analytics literally as posed.
- Building block for largest-rectangle-in-histogram and trapping-rain-water style problems.

## Cousins & contrasts

- **sliding_window_max**: the deque sibling — monotonic *queue* for a moving-window max, versus a monotonic *stack* for next-greater distances. Same push/pop-at-most-once argument.
- **Largest rectangle in histogram**: monotonic *increasing* stack, resolves on a *shorter* bar — the mirror image of this one.
- **Next greater element II (circular array)**: same stack, just iterate the array twice and take indices mod n.
