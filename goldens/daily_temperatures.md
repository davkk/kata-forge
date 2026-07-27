# Daily Temperatures

For each day, how many days until a warmer temperature -- 0 if none. Solved in O(n) with a **monotonic stack** of unresolved days.

## Intuition

- Keep a stack of indices whose temperatures are **strictly decreasing** -- days still waiting for a warmer one.
- When day `i` is warmer than the stack top, it is *the* answer for that top: pop and record `i - idx`. One warm day can resolve a whole run of colder pending days.
- Then push `i` itself: it now waits for its own warmer day.
- The decreasing invariant means the stack top is always the *nearest unresolved day that `i` can resolve* -- deeper entries are colder still and must keep waiting.
- Amortized O(n): each index is pushed once and popped at most once -> at most 2n stack operations.

## Approach -- monotonic stack of indices

```cpp
using namespace std;

vector<int> daily_temperatures(const vector<int>& t) {
    vector<int> out(t.size(), 0);
    vector<int> st;
    for (int i = 0; i < (int)t.size(); ++i) {
        while (!st.empty() && t[st.back()] < t[i]) {
            int idx = st.back(); st.pop_back();
            out[idx] = i - idx;
        }
        st.push_back(i);
    }
    return out;
}
```

### Walkthrough

On `t = [73, 74, 75, 71, 69, 72, 76, 73]`:
- i=0: stack [], push 0 -> [0]
- i=1: t[0]=73 < 74 -> pop 0, out[0]=1; push 1 -> [1]
- i=2: pop 1, out[1]=1; push 2 -> [2]
- i=3: 75 not > 71, push 3 -> [2, 3]
- i=4: 75 not > 69, push 4 -> [2, 3, 4]
- i=5 (72): pop 4 (out[4]=1), pop 3 (out[3]=2); push 5 -> [2, 5]
- i=6 (76): pop 5 (out[5]=1), pop 2 (out[2]=4); push 6 -> [6]
- i=7 (73): push 7 -> [6, 7]
- return out = [1, 1, 4, 2, 1, 1, 0, 0]

- Push **indices, not temperatures** -- the answer is a distance `i - idx`, not a value.
- Strict `<` in the pop condition: equal temperatures don't resolve (the question asks for *warmer*, not "at least as warm").
- Indices left on the stack never see a warmer day -- their 0 defaults are already correct, so no cleanup pass is needed.

## Complexity

- Time: O(n) -- amortized, each index pushed and popped at most once.
- Space: O(n) for the stack in the worst case.

## Alternative -- brute force

- From each day, scan right until something warmer appears -- O(n^2) worst case (strictly decreasing input never resolves early).
- The stack is the same search with memory: every "known colder" day is parked by position and resolved in O(1) when its answer arrives.

## Alternative -- reverse pass (no stack)

- Walk right-to-left. For each day, skip ahead to the next warmer day using a separate "next warmer" array filled during the same pass.
- Same O(n) time and O(n) space, no stack discipline to maintain. Reads less naturally but is a useful "no-stack" version of the same idea.

## Usage

- The entire "next greater element" family: stock span, next-higher price, signal-peak distances.
- Weather and climate analytics, literally as posed.
- Building block for largest-rectangle-in-histogram and trapping-rain-water.
- Anywhere a stream of values needs "how far ahead until the next one that beats me".

## Cousins & contrasts

- **Sliding window maximum**: the deque sibling -- monotonic *queue* for a moving-window max, versus a monotonic *stack* for next-greater distances. Same push/pop-at-most-once argument.
- **Largest rectangle in histogram**: monotonic *increasing* stack, resolves on a *shorter* bar -- the mirror image of this one.
- **Next greater in a circular array**: same stack, iterate the array twice and take indices mod n.
- **Brute force scan**: O(n^2) -- the baseline the monotonic stack replaces.
