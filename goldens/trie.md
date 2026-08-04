# Trie -- one edge per character: a word is a walk from the root

## Core idea

- Invariant: a node *is* a prefix; a word exists iff the character walk ends on a node with `isWord` set -- shared prefixes share nodes, no hashing, no collisions.
- Mechanism: every op walks one path of length L (or a subtree), so cost is O(L + output size), independent of how many words are stored.

## Build up

1. **Each edge is one character**

```
n->child[c - 'a']      // follow the edge for char c
n->isWord              // flag: a word ends here
```

2. **Insert: walk, create missing edges**

```
for (char c : word) {
    if (!n->child[c - 'a']) n->child[c - 'a'] = new Node();
    n = n->child[c - 'a'];
}
n->isWord = true;
```

3. **Remove: clear the flag only**

```
for (char c : word) { n = n->child[c - 'a']; if (!n) return; }
n->isWord = false;      // nodes stay: other words may own them
```

4. **Find: walk the prefix, DFS below**

```
for (char c : prefix) { n = n->child[c - 'a']; if (!n) return {}; }
collect(n, p, out);     // flag -> emit; recurse all children in order
```

## Diagram

```
insert "hello", then "help"            find("hel"): walk h-e-l, DFS below

      h
      |
      e
      |
      l        <- walk stops here
     / \
    l   p          l -> "hello"   (isWord)
    |              p -> "help"    (isWord)
    o
```

## Approach -- array children (a-z)

```cpp
using namespace std;

class Trie {
public:
    Node* root;

    Trie() : root(new Node()) {}

    void insert(const string& word) {
        Node* n = root;
        for (char c : word) {                       // step 2: walk
            int i = c - 'a';
            if (!n->child[i]) n->child[i] = new Node();
            n = n->child[i];
        }
        n->isWord = true;                           // step 2: mark end
    }

    void remove(const string& word) {
        Node* n = root;
        for (char c : word) {
            n = n->child[c - 'a'];
            if (!n) return;                         // step 3: not stored
        }
        n->isWord = false;                          // step 3: clear flag
    }

    void collect(Node* n, string& prefix, vector<string>& out) {
        if (n->isWord) out.push_back(prefix);
        for (int i = 0; i < 26; ++i)
            if (n->child[i]) {
                prefix.push_back('a' + i);
                collect(n->child[i], prefix, out);  // step 4: DFS below
                prefix.pop_back();
            }
    }

    vector<string> find(const string& prefix) {
        Node* n = root;
        for (char c : prefix) {                     // step 4: walk
            n = n->child[c - 'a'];
            if (!n) return {};
        }
        vector<string> out;
        string p = prefix;
        collect(n, p, out);
        return out;
    }
};
```

- Steps 1-4 assembled: the edge walk (1), insert creates edges + flags (2), remove clears the flag (3), find walks then collects (4).
- Remove never deletes nodes -- other words may own them; an unmarked node is harmless dead weight.

### Trace

- insert "hello", "help" share the h-e-l spine; find("hel") -> ["hello","help"] in alphabetical order.
- remove("help") clears the p flag; find("hel") -> ["hello"]; find("xyz") -> {} (missing x child).

## Complexity

- Time: O(L) per insert/remove/find, O(L + output) for prefix enumeration. Space: O(total characters stored).

## Alternative -- hash-map children

- `unordered_map<char, Node*>` per node: memory follows actual branching, any alphabet; array children are faster and iterate in order for a-z.

## Use when

- Reach for this when queries are "everything under a prefix" or longest-prefix-match -- exact lookup alone is not the question.
- Autocomplete, spell checkers, word-search games: one missing child prunes a whole board branch.
- IP routing: longest-prefix match is a bitwise trie walk.

## Cousins

- **Radix tree**: collapses single-child chains into one labeled edge -- same queries, far fewer nodes.
- **Suffix tree**: trie of all suffixes of one string -- substring queries in O(L).
- **Hash set of words**: same O(L) exact match, but cannot answer prefix queries at all.
- **DAWG**: also merges suffixes -- minimal automaton, harder to build.
