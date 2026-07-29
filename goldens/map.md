# Hash Map

A key to value dictionary with O(1) average get/set/delete. Same hashing core as a hash set — bucket entries just carry a value next to the key, and `set` on an existing key **replaces its value instead of being a no-op**.

## Intuition

- `index = hash(key) % capacity`, one vector per bucket for collisions (separate chaining), rehash at load factor ~= 0.75 — see the hash set golden; nothing changes.
- The one new move: every bucket walk compares keys, and on a match either reads (`get`), overwrites (`set`), or unlinks (`erase`) the entry it found.
- Average O(1) per op for the same reason: uniform hash implies expected chain length is the small constant load factor.
- String keys: a polynomial rolling hash (`h = h*31 + c`) mixes position into the hash, so anagrams don't all collide — a plain character sum would.

## Approach — separate chaining with KV entries

```cpp
using namespace std;

struct Map {
    vector<vector<KV>> buckets = vector<vector<KV>>(16);
    int count = 0;

    unsigned hash_of(const string& k) {
        unsigned h = 0;
        for (char c : k) h = h * 31 + (unsigned char)c;
        return h % buckets.size();
    }

    void set(const string& k, int v) {
        int idx = hash_of(k);
        for (auto& kv : buckets[idx])
            if (kv.key == k) { kv.val = v; return; }
        if (count >= (int)buckets.size() * 3 / 4) {
            rehash();
            idx = hash_of(k);
        }
        buckets[idx].push_back({k, v});
        count++;
    }

    optional<int> get(const string& k) {
        for (auto& kv : buckets[hash_of(k)])
            if (kv.key == k) return kv.val;
        return nullopt;
    }

    void erase(const string& k) {
        auto& bucket = buckets[hash_of(k)];
        for (int i = 0; i < (int)bucket.size(); i++)
            if (bucket[i].key == k) {
                bucket.erase(bucket.begin() + i);
                count--;
                return;
            }
    }

    int size() {
        return count;
    }

    void rehash() {
        vector<vector<KV>> old = buckets;
        buckets.assign(old.size() * 2, {});
        for (auto& bucket : old)
            for (auto& kv : bucket)
                buckets[hash_of(kv.key)].push_back(kv);
    }
};
```

- `set` checks for the key *before* inserting: found -> overwrite; missing -> push back a new entry. That distinction is what keeps `count` honest.
- Pitfall: recompute `idx` after `rehash` — a reference captured before the resize dangles. `get` uses `optional<int>`: `nullopt` means missing, `optional(0)` means stored 0.
- `erase` shifts O(k) — swap-with-last + `pop_back` avoids it. `rehash` rebuilds by iterating old buckets and re-inserting via `push_back`.

## Alternative — balanced BST (`std::map`)

- Keep keys in a red-black tree: every op is O(log n) *worst case*, not just average — no hash function to go wrong.
- You also get ordered iteration and range queries (`lower_bound`, "all keys in [a, b]") for free; a hash map gives none.
- Pay with a bigger constant factor and no O(1) average — pick it when order matters or adversarial input is a threat.

## Alternative — open addressing

- Same idea as the hash set alternative: no pointers per entry, all in one array, probe on collision.
- Tombs for deletion, linear/quadratic/double hashing for probing, worst-case behavior on heavy load.
- The choice for cache-sensitive workloads (database buffer pools, hot in-memory indexes).

## Complexity

- Time: O(1) average for set/get/erase/size, O(n) worst case with a hostile hash.
- Space: O(n).

## Usage
- Associative arrays, memoization caches, frequency counting, index-of-x tables.
- Two-sum: complement lookup turns O(n^2) into one pass.
- Any fast "look up by key, insert, delete" workload.

## Cousins & contrasts

- **`std::unordered_map` vs `std::map`**: chained hashing (O(1) average, unordered) vs red-black tree (O(log n) worst, ordered). In interviews, say which you want and why.
- **Hash set**: this file minus the `val` field — a set is just a map with ignored values.
- **Multimap**: allows duplicate keys; bucket entries (or tree) hold one entry per pair instead of replacing.
- **Trie**: structure-of-prefixes alternative when keys are strings and prefix queries matter.
