# Hash Set -- one bucket per key: membership scans only that bucket

## Core idea

- Invariant: a key lives in exactly one bucket, `hash(key) % capacity`, so contains/add/remove scan that bucket alone -- never the whole set.
- O(1) average is a contract, not a property: a uniform hash plus a capped load factor (double the table on rehash) keeps every chain a small constant.

## Build up

1. **The key chooses its bucket**

```
int idx = hash_of(x);                    // hash(x) % buckets.size()
```

2. **Membership: scan one chain**

```
for (int v : buckets[idx]) if (v == x) return true;
```

3. **Add: dedup, then push**

```
if (contains(x)) return;                 // set semantics: no duplicates
buckets[hash_of(x)].push_back(x);
```

4. **Resize before chains grow long**

```
if (count >= cap * 3 / 4) rehash();      // double, re-scatter every key
```

## Diagram

```
cap = 8, hash = k % 8
add 12, 9, 5, 4, 22        idx:  0   1   2   3    4    5   6   7
                                []  [9] []  [] [12,4] [5] [22] []
contains(4): hash 4 -> bucket 4, scan: 12? no, 4? yes -> true
add 9 again: contains hits, nothing pushed, count stays 5
```

## Approach -- separate chaining

```cpp
using namespace std;

class HashSet {
public:
    vector<vector<int>> buckets;
    int count;

    HashSet() : buckets(16), count(0) {}

    int hash_of(int k) {
        unsigned h = (unsigned)k * 2654435761u;
        return (int)(h % buckets.size());       // step 1: scatter
    }

    void add(int x) {
        if (contains(x)) return;                // step 3: dedup
        if (count >= (int)buckets.size() * 3 / 4) rehash(); // step 4
        buckets[hash_of(x)].push_back(x);       // step 3: push
        count++;
    }

    void remove(int x) {
        auto& bucket = buckets[hash_of(x)];
        for (int i = 0; i < (int)bucket.size(); i++)
            if (bucket[i] == x) {
                bucket.erase(bucket.begin() + i);
                count--;
                return;
            }
    }

    bool contains(int x) {
        for (int v : buckets[hash_of(x)])       // step 2: scan one chain
            if (v == x) return true;
        return false;
    }

    int size() {
        return count;
    }

    void rehash() {
        vector<vector<int>> old = buckets;
        buckets.assign(old.size() * 2, {});     // step 4: double
        for (auto& bucket : old)
            for (int v : bucket)
                buckets[hash_of(v)].push_back(v); // step 4: re-scatter
    }
};
```

- Steps 1-4 assembled: `hash_of` scatters (1), `contains` scans one chain (2), `add` dedups then pushes (3), `rehash` doubles the table (4).
- `count` is maintained so `size()` is O(1); `remove` erases from the one chain it finds the key in.

### Trace

- add 12, 9, 5, 22 -> buckets 4, 1, 5, 6; add 5 again -> `contains` hits, count stays 4.
- contains(9) -> bucket 1, one compare -> true; remove(5) -> bucket 5 erase -> count 3.

## Complexity

- Time: O(1) average per op (O(n) worst with a hostile hash). Space: O(n).

## Alternative -- open addressing

- Store entries in the array itself and probe `idx+1, idx+2, ...` on collision: no per-entry allocation, cache-friendlier.
- Costs: tombstones for delete, long probe runs (clustering) past ~0.7 load -- chaining degrades more gracefully.

## Use when

- Reach for this when you need fast add/remove/membership of distinct keys and never need order or ranges.
- Dedup streams, "seen before" checks, two-sum complement lookups, visited-URL sets for crawlers.

## Cousins

- **Hash map**: identical skeleton, each bucket entry just carries a value next to the key (see map).
- **std::set (red-black)**: O(log n) worst case, ordered iteration; this is O(1) average, unordered.
- **Bloom filter**: tiny probabilistic membership -- may say "maybe", never "definitely not".
- **Open addressing**: same scatter, collisions resolved by probing instead of chains.
