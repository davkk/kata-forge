# LRU Cache

A fixed-capacity cache that evicts the **least recently used** entry, with O(1) get and update. Neither a hash map nor a linked list alone can do this — the classic composite structure.

## Intuition

- Hash map alone: O(1) lookup, but no notion of "oldest" — finding the evict victim costs O(n).
- Linked list alone: O(1) reorder and eviction at the ends, but finding a key costs O(n).
- Composite: `unordered_map<key, Node*>` answers *where* in O(1); the doubly linked list holds nodes in recency order and answers *what to evict / how to reorder* in O(1).
- Invariant: **head = most recently used, tail = evict victim**. Every access (get *and* update) moves its node to the head.
- Doubly linked (not singly) because detaching an arbitrary node needs its predecessor — with `prev` pointers it's O(1).
- All ops O(1): a constant number of pointer swaps and one hash lookup; space O(capacity).

## Hash map + doubly linked list

```cpp
using namespace std;

struct Node { string key; int val; Node *prev, *next; };

struct LRU {
    int cap;
    unordered_map<string, Node*> m;
    Node *head = nullptr, *tail = nullptr;   // head = MRU, tail = victim
};

void detach(LRU& c, Node* n) {
    if (n->prev) n->prev->next = n->next; else c.head = n->next;
    if (n->next) n->next->prev = n->prev; else c.tail = n->prev;
}

void prepend(LRU& c, Node* n) {
    n->prev = nullptr; n->next = c.head;
    if (c.head) c.head->prev = n; else c.tail = n;
    c.head = n;
}

bool get(LRU& c, const string& k, int& out) {
    auto it = c.m.find(k);
    if (it == c.m.end()) return false;
    detach(c, it->second); prepend(c, it->second);   // touch: now most recent
    out = it->second->val;
    return true;
}

void update(LRU& c, const string& k, int v) {
    auto it = c.m.find(k);
    if (it != c.m.end()) {                           // replace value + touch
        it->second->val = v;
        detach(c, it->second); prepend(c, it->second);
        return;
    }
    if ((int)c.m.size() == c.cap) {                  // evict tail first
        Node* dead = c.tail;
        c.m.erase(dead->key);                        // read key before delete
        detach(c, dead);
        delete dead;
    }
    Node* n = new Node{k, v, nullptr, nullptr};
    c.m[k] = n;
    prepend(c, n);
}
```

- Touching = `detach` + `prepend` — write these two helpers once and every op becomes trivial.
- Eviction must erase from the map *and* the list; forgetting `m.erase` leaves a dangling pointer that a later get will follow.
- Pitfall: read `tail->key` before `delete dead`, and don't touch when the map misses (`get` on absent key changes nothing).
- Single-node edge cases fall out naturally if detach/prepend handle `head == tail == n`.

## Alternative: std::list + map of iterators (interview shorthand)

- `list<pair<K,V>>` (front = MRU) plus `unordered_map<K, list<...>::iterator>`; list iterators stay valid across moves, so no raw-pointer surgery.
- Touch is `l.splice(l.begin(), l, it->second)` — O(1), no detach code at all.
- Same complexity, far less code; the handwritten version above is what interviewers probe for when they say "no library list".

## Where it shows up

- Browser and CDN caches, database buffer pools, Redis (`allkeys-lru`), OS page-replacement approximations.
- Bounding memory of memoization: cache the last k results, evict the rest.
- Anywhere "keep the hot working set, drop the coldest" is the policy.

## Cousins & contrasts

- **FIFO cache**: evicts the oldest *insertion*, never reorders on get — simpler (a queue suffices) but punishes popular old keys.
- **LFU cache**: evicts least *frequently* used; needs per-key counters plus min-tracking, noticeably more machinery.
- **Second chance / clock**: FIFO + a reference bit, the OS-friendly approximation of LRU with no list surgery.
- **Direct-mapped CPU cache**: eviction by hash collision, not recency — the hardware cousin with O(1) everything and no policy at all.
