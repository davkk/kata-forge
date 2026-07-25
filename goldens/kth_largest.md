# Kth Largest Element

Return the k-th largest element of an unsorted array (1-based k). Two canonical answers: a size-k min-heap, or quickselect.

## Intuition

- Sorting solves it in O(n log n) but does more work than needed — the order of the other n−k elements is irrelevant.
- Heap view: keep the **k largest elements seen so far** in a min-heap. Its top is the smallest of those k — by definition the kth largest overall.
- Selection view: one partition pass places the pivot at its final sorted position; if that's the target index we're done, else recurse into only the side containing it.
- Heap: O(n log k) time, O(k) space, stream-friendly. Quickselect: O(n) average (n + n/2 + n/4 + … ≈ 2n), O(1) extra space, but mutates the array.

## Min-heap of size k (stream-friendly)

```cpp
using namespace std;

int kth_largest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<>> heap;  // min-heap
    for (int x : nums) {
        heap.push(x);
        if ((int)heap.size() > k) heap.pop();          // evict smallest
    }
    return heap.top();                                 // kth largest
}
```

- The heap never exceeds k, so each step costs O(log k), not O(log n).
- Pop-when-full is the correctness core: once k larger elements exist, a smaller one can never re-enter the top k.

## Alternative: quickselect (average O(n), mutates input)

- Lomuto partition like quicksort, but recurse into **one side only** — that asymmetry is the whole drop from O(n log n) to O(n).
- Target index is `n - k` in ascending order (kth largest = (n−k)th smallest).
- Pitfall: a fixed pivot (e.g. always the last element) degrades to O(n²) on sorted input; a random pivot makes the worst case vanishingly unlikely.

```cpp
int kth_largest(vector<int>& nums, int k) {
    int lo = 0, hi = (int)nums.size() - 1, target = (int)nums.size() - k;
    while (true) {
        int p = lo + rand() % (hi - lo + 1);           // random pivot → hi
        swap(nums[p], nums[hi]);
        int pivot = nums[hi], i = lo;
        for (int j = lo; j < hi; ++j)                  // Lomuto partition
            if (nums[j] <= pivot) swap(nums[i++], nums[j]);
        swap(nums[i], nums[hi]);                       // pivot at final index i
        if (i == target) return nums[i];
        if (i < target) lo = i + 1;
        else            hi = i - 1;
    }
}
```

## Where it shows up

- Leaderboards, percentile scores, median-of-array (k = n/2) without a full sort.
- `std::nth_element` is exactly quickselect — average linear, element at its sorted position.
- The heap variant powers "top-k of a stream" and external k-way merges where data doesn't fit in memory.

## Cousins & contrasts

- **Sorting**: simplest correct answer, O(n log n); fine for a one-shot query, wasteful when k is small.
- **min_heap**: the heap variant is one insert/extract-min loop on top of the hand-rolled structure.
- **Quicksort**: identical partition; quicksort recurses on both sides, quickselect on one — the entire complexity difference.
- **Order-statistics tree**: O(log n) kth queries on a *changing* set, at the cost of maintaining a balanced BST with subtree sizes.
