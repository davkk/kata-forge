# LRU cache -- map finds, list orders: compose two O(1) structures for eviction

## Core idea

- Invariant: the list holds live keys in recency order -- head is most recently used, tail is the eviction victim; every access moves its node to the head.
- Mechanism: the map answers "where is the node" in O(1), the list answers "what is oldest" in O(1); eviction deletes from both, so lookup and order stay consistent.

## Build up

1. **Two halves, one gap**

```
unordered_map<string, Node*> m;   // O(1) lookup, no "oldest"
Node* head, *tail;                // order, but finding a key is O(n)
```

2. **Map points at list nodes**

```
m[key] = n;   // n also sits in the recency list
```

3. **touch: detach, then prepend**

```
detach(n);    // splice out of the middle
prepend(n);   // wire to the head (MRU)
```

4. **evict: drop tail, erase map**

```
m.erase(tail->key);   // Node stores its key
detach(tail);
delete tail;
```

## Diagram

```
cap 2
update a, update b      list: b <-> a       tail = victim
get("a") touches        list: a <-> b       a is now MRU
update c (full)         evict b             map {a, c}, list c <-> a
get("b")                nullopt             (evicted)
```

## Approach -- hash map + doubly linked list

```cpp
using namespace std;

class LRU {
public:
    int cap;
    unordered_map<string, Node*> m;   // step 2
    Node* head;                       // head = MRU
    Node* tail;                       // tail = victim

    LRU(int cap) : cap(cap), m(), head(nullptr), tail(nullptr) {}

    optional<int> get(const string& k) {
        auto it = m.find(k);
        if (it == m.end()) return nullopt;
        touch(it->second);            // step 3
        return it->second->val;
    }

    void update(const string& k, int v) {
        auto it = m.find(k);
        if (it != m.end()) {
            it->second->val = v;
            touch(it->second);
            return;
        }
        if ((int)m.size() == cap) evict();   // step 4
        Node* n = new Node{k, v, nullptr, nullptr};
        m[k] = n;                            // step 2
        prepend(n);
    }

    int size() { return (int)m.size(); }

    void touch(Node* n) { detach(n); prepend(n); }   // step 3

    void detach(Node* n) {
        if (n->prev) n->prev->next = n->next; else head = n->next;
        if (n->next) n->next->prev = n->prev; else tail = n->prev;
    }

    void prepend(Node* n) {
        n->prev = nullptr; n->next = head;
        if (head) head->prev = n; else tail = n;
        head = n;
    }

    void evict() {                                   // step 4
        Node* dead = tail;
        m.erase(dead->key);
        detach(dead);
        delete dead;
    }
};
```

- The map entry and the list node are the same object per key -- that identity joins the two halves (steps 1-2).
- `get` and `update` both call `touch`: a read changes recency, so reads must reorder (step 3).
- `evict` cleans both halves; the stored key makes `m.erase` possible (step 4).

### Trace

- cap 2: update a=1, b=2 -> list `b <-> a`; get("a") touches -> list `a <-> b`; update c=3 evicts b from list and map; get("b") -> nullopt, get("a") -> 1 and a becomes MRU.

## Complexity

- Time: O(1) per get, update, size. Space: O(cap) -- one node plus one map entry per live key.

## Alternative -- std::list + map of iterators

- `list<pair<string,int>>` plus `unordered_map<string, iterator>`; `splice` to the front does the touch -- same complexity, no raw-pointer surgery (the handwritten version is what interviews mean by "no library list").

## Use when

- Reach for this when you need "lookup by key" AND "evict the oldest access", both O(1) -- caches, bounded memoization.
- Pattern trigger: "cache of size N, drop least recently used" -> hash map + linked list (a heap, if eviction is by urgency).

## Cousins

- **FIFO cache**: evicts oldest insertion; reads never reorder.
- **LFU cache**: evicts least frequent -- counters plus a heap.
- **Clock/second chance**: LRU approximation with a reference bit, OS-style.
- **Doubly linked list**: the order half in isolation (see doubly_linked_list).
