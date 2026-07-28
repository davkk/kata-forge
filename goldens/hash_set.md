# Hash Set

Stores distinct keys with O(1) average add/remove/contains by scattering keys across an array of buckets. Everything — correctness of the complexity claim and real-world speed — hinges on the **hash function spreading keys uniformly**.

## Intuition

- `index = hash(key) % capacity`: the key itself decides its bucket, so a lookup only scans that one bucket instead of the whole set.
- Collisions are unavoidable (pigeonhole: more possible keys than buckets), so each bucket holds a small vector — *separate chaining* with vector buckets.
- Average O(1): with a uniform hash, expected chain length is the load factor `n / capacity` — a small constant, not a function of n.
- Rehash at load factor ~= 0.75: double the capacity and re-link every element. One rehash is O(n), but it buys ~n cheap inserts before the next, so the amortized cost per op stays O(1).
- Worst case is still O(n): all keys land in one bucket. That is what a bad hash (or adversarial input) does to you.

## Approach — separate chaining

```cpp
using namespace std;

struct HashSet {
    vector<vector<int>> buckets;
    int count = 0;

    int hash_of(int k) {
        unsigned h = (unsigned)k * 2654435761u;
        return (int)(h % buckets.size());
    }

    void add(int x) {
        if (contains(x)) return;
        if (count >= (int)buckets.size() * 3 / 4) rehash();
        buckets[hash_of(x)].push_back(x);
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
        for (int v : buckets[hash_of(x)])
            if (v == x) return true;
        return false;
    }

    int length() {
        return count;
    }

    void rehash() {
        vector<vector<int>> old = buckets;
        buckets.assign(old.size() * 2, {});
        for (auto& bucket : old)
            for (int v : bucket)
                buckets[hash_of(v)].push_back(v);
    }
};
```

- `hash_of` uses Knuth's multiplicative hash (magic constant) — simple and fast for int keys.
- `remove` walks the bucket vector and calls `erase` (O(k) shift). Swapping with the last element + `pop_back` avoids the shift but breaks insertion order.
- `buckets` must never be empty or `%` divides by zero; initialize with e.g. 16 empty vectors and grow.
- Pitfall: forgetting to recompute the bucket index after `rehash` — the old index points into a resized array.

## Alternative — open addressing

- Keep all entries in the array itself; on collision probe `idx+1, idx+2, ...` until a free slot. No pointers, no allocation per insert.
- Wins: cache-friendly (one contiguous array), less memory overhead per entry.
- Costs: deletion needs *tombstones* (a plain clear would break later probe chains), and performance collapses past ~0.7 load as probes form long runs (*primary clustering*).
- Chaining degrades gracefully as load grows; open addressing does not — that's why `std::unordered_set` chains.

## Alternative — Robin Hood / cuckoo hashing

- Robin Hood: a colliding key steals the slot of any richer key (greater probe distance), flattening probe lengths and bounding the worst case.
- Cuckoo: two hash functions and two possible slots; on collision kick the existing key out and reinsert it at its alternate slot. O(1) lookups with bounded worst case but harder to implement.

## Complexity

- Time: O(1) average for add/contains/remove, O(n) worst case with a hostile hash.
- Space: O(n), up to ~4n for the array itself when chained.

## Usage

- Duplicate detection: crawlers remembering visited URLs, one-pass "seen before" checks (two-sum style).
- Symbol tables in compilers/interpreters, membership filters ahead of expensive lookups.
- Grouping problems: anagrams, equivalence classes — hash the canonical form, chain the members.
- Any "have I seen this before" question that needs to stay fast as the data grows.

## Cousins & contrasts

- **Hash map**: identical skeleton; bucket entries just carry a value next to the key.
- **Balanced BST set (`std::set`)**: O(log n) *worst case* and ordered iteration; a hash set is O(1) *average* but unordered.
- **Bloom filter**: probabilistic membership in tiny memory — can say "maybe present", never "definitely present".
- **Open addressing**: same hash-index idea, collision resolution by probing instead of chaining (see above).
