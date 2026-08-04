# Daily Temperatures -- days until a warmer day: a monotonic stack of waiting days, each popped once -> O(n)

## Core idea

- Invariant: the stack holds indices of unresolved days with strictly decreasing temperatures; the top is the nearest day still waiting.
- Mechanism: when day i is warmer than the top, day i resolves that top: pop and record the gap `i - top`. Each index is pushed once and popped at most once, so the whole pass is O(n).

## Build up

1. **Naive: scan ahead per day**

```
for (int j = i + 1; j < n; ++j)
    if (a[j] > a[i]) { out[i] = j - i; break; }   // O(n^2) worst
```

2. **Stack of waiting days**

```
while (!st.empty() && a[st.top()] < a[i]) ...
```

3. **Pop when a warmer day arrives**

```
out[st.top()] = i - st.top();   // the gap is the answer
st.pop();
```

4. **Today waits for its own warmer day**

```
st.push(i);
```

## Diagram

```
a = [73 74 75 71 69 72 76 73]
i=0  73                      st [73]
i=1  74  pops 73, out[0]=1   st [74]
i=2  75  pops 74, out[1]=1   st [75]
i=3  71                      st [75 71]
i=5  72  pops 69, out[4]=1   st [75 72]
         pops 71, out[3]=2
i=6  76  pops 72, out[5]=1   st [76]
         pops 75, out[2]=4
i=7  73                      st [76 73]
```

## Approach -- monotonic stack of indices

```cpp
using namespace std;

vector<int> daily_temperatures(const vector<int>& a) {
    vector<int> out(a.size(), 0);
    stack<int> st;
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!st.empty() && a[st.top()] < a[i]) {  // step 2: pop all colder
            int idx = st.top(); st.pop();
            out[idx] = i - idx;       // step 3: the gap is the answer
        }
        st.push(i);                   // step 4: today waits too
    }
    return out;
}
```

- Store indices, not temperatures: the answer is a distance `i - idx`, not a value.
- Strict `<`: equal temperatures do not resolve, since the question asks for *warmer*.
- Days left on the stack never see a warmer day; their zero defaults are already correct.

### Trace

- `[73 74 75 71 69 72 76 73]` -> `[1 1 4 2 1 1 0 0]`; day 2 (75) waits four days for 76.

## Complexity

- Time: O(n), each index pushed and popped at most once. Space: O(n) for the stack.

## Alternative -- brute force

- From each day, scan right until something warmer appears: O(n^2) on decreasing input; the stack is the same search with memory.

## Use when

- Any "distance to the next value that beats me" query: stock span, next higher price, signal peaks.
- Reach for this when the answer is the offset of the next greater element, not the element itself.
- Monotonic stacks are also the base of largest-rectangle-in-histogram and trapping rain water.

## Cousins

- **Sliding window maximum**: a monotonic deque for a moving window, same push/pop-once argument.
- **Largest rectangle in histogram**: increasing stack resolved by a shorter bar, the mirror image.
- **Circular next greater**: same stack, run the pass twice with indices mod n.
