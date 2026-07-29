# LRU Cache

A fixed-capacity cache that evicts the **least recently used** entry, with O(1) get and update. Neither a hash map nor a linked list alone can do this — the classic composite structure.

## Intuition

- Hash map alone: O(1) lookup, but no notion of "oldest" — finding the evict victim costs O(n).
- Linked list alone: O(1) reorder and eviction at the ends, but finding a key costs O(n).
- Composite: `unordered_map<key, Node*>` answers *where* in O(1); the doubly linked list holds nodes in recency order and answers *what to evict / how to reorder* in O(1).
- Invariant: **head = most recently used, tail = evict victim**. Every access (get *and* update) moves its node to the head.
- Doubly linked (not singly) because detaching an arbitrary node needs its predecessor — with `prev` pointers it is O(1).
- All ops O(1): a constant number of pointer swaps and one hash lookup; space O(capacity).

## Approach — hash map + doubly linked list

```cpp
using namespace std;

struct Node {
    string key;
    int val;
    Node *prev, *next;
};

struct LRU {
    int cap;
    unordered_map<string, Node*> m;
    Node *head = nullptr, *tail = nullptr;   // head = MRU, tail = victim

    optional<int> get(const string& k) {
        auto it = m.find(k);
        if (it == m.end()) return nullopt;
        touch(it->second);
        return it->second->val;
    }

    void update(const string& k, int v) {
        auto it = m.find(k);
        if (it != m.end()) {
            it->second->val = v;
            touch(it->second);
            return;
        }
        if ((int)m.size() == cap) evict();
        Node* n = new Node{k, v, nullptr, nullptr};
        m[k] = n;
        prepend(n);
    }

    int size() { return (int)m.size(); }

    void touch(Node* n) { detach(n); prepend(n); }

    void detach(Node* n) {
        if (n->prev) n->prev->next = n->next; else head = n->next;
        if (n->next) n->next->prev = n->prev; else tail = n->prev;
    }

    void prepend(Node* n) {
        n->prev = nullptr; n->next = head;
        if (head) head->prev = n; else tail = n;
        head = n;
    }

    void evict() {
        Node* dead = tail;
        m.erase(dead->key);
        detach(dead);
        delete dead;
    }
};
```

### Walkthrough (`cap = 2`)
- `update("a",1)`: map={a:1}, list=[a:1]; `update("b",2)`: map={a:1,b:2}, list=[b:2]<->[a:1] (b newest)
- `get("a")` -> 1: detach a from tail, prepend to head; list=[a:1]<->[b:2]
- `update("c",3)`: cap full, evict tail b, prepend c; map={a:1,c:3}, list=[c:3]<->[a:1]
- `get("b")` -> nullopt (evicted); `get("a")` -> 1: touch; list=[a:1]<->[c:3]
- Touching = `detach` + `prepend`. Eviction must erase from both map and list.
- Pitfall: read `tail->key` before `delete dead`; single-node edge cases fall out naturally.
## Complexity
- Time: O(1) per get, update, size.
- Space: O(capacity).
## Alternative — `std::list` + map of iterators (interview shorthand)
- `list<pair<K,V>>` (front = MRU) plus `unordered_map<K, list<...>::iterator>`; list iterators stay valid across moves, so no raw-pointer surgery.
- Touch is `l.splice(l.begin(), l, it->second)` — O(1), no detach code at all.
- Same complexity, far less code; the handwritten version above is what interviewers probe for when they say "no library list".
## Alternative — array + timestamps (low-overhead bounded cache)
- For a small, fixed key set, store entries in an array indexed by hash and use a per-slot timestamp (counter) for LRU.
- No allocation, no pointer chasing; wins on cache misses for the entire cache.
- Loses to the linked list when capacity varies or eviction must be exact.
## Usage
- Browser and CDN caches, database buffer pools, Redis (`allkeys-lru`).
- Bounding memory of memoization: cache the last k results, evict the rest.

## Cousins & contrasts
- **FIFO cache**: evicts oldest insertion, never reorders on get — simpler but punishes popular old keys.
- **LFU cache**: evicts least frequently used; needs per-key counters and min-tracking.
- **Second chance / clock**: FIFO + reference bit, OS-friendly LRU approximation with no list surgery.
- **Direct-mapped CPU cache**: eviction by hash collision, not recency.
