# Hash Map

A key→value dictionary with O(1) average get/set/delete. Same hashing core as a hash set — bucket nodes just carry a value next to the key, and `set` on an existing key **replaces its value instead of being a no-op**.

## Intuition

- `index = hash(key) % capacity`, one linked list per bucket for collisions (separate chaining), rehash at load factor ≈ 0.75 — see the hash set golden; nothing changes.
- The one new move: every bucket walk compares keys, and on a match either reads (`get`), overwrites (`set`), or unlinks (`delete`) the node it found.
- Average O(1) per op for the same reason: uniform hash ⇒ expected chain length is the small constant load factor.
- String keys: a polynomial rolling hash (`h = h*31 + c`) mixes position into the hash, so anagrams don't all collide — a plain character sum would.

## Separate chaining with key/value nodes

```cpp
using namespace std;

struct Node { string key; int val; Node* next; };

struct HashMap {
    vector<Node*> buckets;    // initialize with e.g. 16 nulls, never empty
    int count = 0;
};

int hash_of(const HashMap& m, const string& k) {
    unsigned h = 0;
    for (char c : k) h = h * 31 + (unsigned char)c;    // position-sensitive
    return (int)(h % m.buckets.size());
}

bool get(HashMap& m, const string& k, int& out) {
    for (Node* cur = m.buckets[hash_of(m, k)]; cur; cur = cur->next)
        if (cur->key == k) { out = cur->val; return true; }
    return false;
}

void rehash(HashMap& m) {
    vector<Node*> old = m.buckets;
    m.buckets.assign(old.size() * 2, nullptr);
    for (Node* head : old)
        while (head) {
            Node* nxt = head->next;
            int idx = hash_of(m, head->key);
            head->next = m.buckets[idx];
            m.buckets[idx] = head;
            head = nxt;
        }
}

void set(HashMap& m, const string& k, int v) {
    int idx = hash_of(m, k);
    for (Node* cur = m.buckets[idx]; cur; cur = cur->next)
        if (cur->key == k) { cur->val = v; return; }   // replace in place
    if (m.count >= (int)m.buckets.size() * 3 / 4) {
        rehash(m);
        idx = hash_of(m, k);                           // buckets moved!
    }
    m.buckets[idx] = new Node{k, v, m.buckets[idx]};
    m.count++;
}

void erase(HashMap& m, const string& k) {
    int idx = hash_of(m, k);
    for (Node** pp = &m.buckets[idx]; *pp; pp = &(*pp)->next)
        if ((*pp)->key == k) {
            Node* dead = *pp;
            *pp = dead->next;
            delete dead;
            m.count--;
            return;
        }
}
```

- `set` checks for the key *before* inserting: found → overwrite; missing → prepend a new node. That distinction is what keeps `count` honest.
- Pitfall: recompute `idx` after rehashing — the old index points into a resized array.
- Pitfall: `get` needs a found/missing signal separate from the value (here via out-param + bool); a sentinel like 0 can't express "stored 0".
- `Node** pp` again: unlinking through a pointer-to-pointer handles the bucket head with no special case.

## Alternative: balanced BST (`std::map`)

- Keep keys in a red-black tree: every op is O(log n) *worst case*, not just average — no hash function to go wrong.
- You also get ordered iteration and range queries (`lower_bound`, "all keys in [a, b]") for free; a hash map gives none.
- Pay with a bigger constant factor and no O(1) average — pick it when order matters or adversarial input is a threat.

## Where it shows up

- Associative arrays and object/dict literals in every dynamic language; JSON objects under the hood.
- Memoization caches, frequency counting, index-of-x tables, grouping records by a field.
- Two-sum and friends: complement lookup turns an O(n²) pair search into one pass.

## Cousins & contrasts

- **`std::unordered_map` vs `std::map`**: chained hashing (O(1) average, unordered) vs red-black tree (O(log n) worst, ordered). In interviews, say which you want and why.
- **Hash set**: this file minus the `val` field — a set is just a map with ignored values.
- **Multimap**: allows duplicate keys; bucket nodes (or tree) hold one entry per pair instead of replacing.
