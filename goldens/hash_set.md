# Hash Set

Stores distinct keys with O(1) average add/remove/contains by scattering keys across an array of buckets. Everything — correctness of the complexity claim and real-world speed — hinges on the **hash function spreading keys uniformly**.

## Intuition

- `index = hash(key) % capacity`: the key itself decides its bucket, so a lookup only scans that one bucket instead of the whole set.
- Collisions are unavoidable (pigeonhole: more possible keys than buckets), so each bucket holds a small linked list — *separate chaining*.
- Average O(1): with a uniform hash, expected chain length is the load factor `n / capacity` — a small constant, not a function of n.
- Rehash at load factor ≈ 0.75: double the capacity and re-link every node. One rehash is O(n), but it buys ~n cheap inserts before the next, so the amortized cost per op stays O(1).
- Worst case is still O(n): all keys land in one bucket. That is what a bad hash (or adversarial input) does to you.

## Separate chaining

```cpp
using namespace std;

struct Node { int key; Node* next; };

struct HashSet {
    vector<Node*> buckets;    // initialize with e.g. 16 nulls, never empty
    int count = 0;
};

int hash_of(const HashSet& s, int k) {
    return (int)(((unsigned)k * 2654435761u) % s.buckets.size()); // Knuth multiplicative
}

bool contains(const HashSet& s, int k) {
    for (Node* cur = s.buckets[hash_of(s, k)]; cur; cur = cur->next)
        if (cur->key == k) return true;
    return false;
}

void rehash(HashSet& s) {
    vector<Node*> old = s.buckets;
    s.buckets.assign(old.size() * 2, nullptr);
    for (Node* head : old)
        while (head) {                       // splice nodes, don't reallocate
            Node* nxt = head->next;
            int idx = hash_of(s, head->key);
            head->next = s.buckets[idx];
            s.buckets[idx] = head;
            head = nxt;
        }
}

void add(HashSet& s, int k) {
    if (contains(s, k)) return;                          // sets keep distinct keys
    if (s.count >= (int)s.buckets.size() * 3 / 4) rehash(s);
    int idx = hash_of(s, k);
    s.buckets[idx] = new Node{k, s.buckets[idx]};        // prepend: O(1), order irrelevant
    s.count++;
}

void remove(HashSet& s, int k) {
    int idx = hash_of(s, k);
    for (Node** pp = &s.buckets[idx]; *pp; pp = &(*pp)->next)
        if ((*pp)->key == k) {
            Node* dead = *pp;
            *pp = dead->next;                  // unlink via pointer-to-pointer
            delete dead;
            s.count--;
            return;
        }
}
```

- Prepend, don't append: buckets are unordered, so the head is always the cheapest insertion point.
- `Node** pp` removal walks a pointer to the *link*, so head removal needs no special case.
- Unsigned multiplication overflow is a feature here — wrapping is free bit-mixing. Don't use signed overflow (UB).
- Pitfall: `buckets` must never be empty or the `%` in `hash_of` divides by zero; start at 16 and grow.
- Pitfall: forgetting to recompute the bucket index after rehash — every op must hash *after* any resize.

## Alternative: open addressing

- Keep all entries in the array itself; on collision probe `idx+1, idx+2, ...` until a free slot. No pointers, no allocation per insert.
- Wins: cache-friendly (one contiguous array), less memory overhead per entry.
- Costs: deletion needs *tombstones* (a plain clear would break later probe chains), and performance collapses past ~0.7 load as probes form long runs (*primary clustering*).
- Chaining degrades gracefully as load grows; open addressing does not — that's why `std::unordered_set` chains.

## Where it shows up

- Duplicate detection: crawlers remembering visited URLs, one-pass "seen before" checks (two-sum style).
- Symbol tables in compilers/interpreters, membership filters ahead of expensive lookups.
- Grouping problems: anagrams, equivalence classes — hash the canonical form, chain the members.

## Cousins & contrasts

- **Hash map**: identical skeleton; bucket nodes just carry a value next to the key.
- **Balanced BST set (`std::set`)**: O(log n) *worst case* and ordered iteration; a hash set is O(1) *average* but unordered.
- **Bloom filter**: probabilistic membership in tiny memory — can say "maybe present", never "definitely present".
- **Open addressing**: same hash-index idea, collision resolution by probing instead of chaining (see above).
