# Trie (Prefix Tree)

A tree where each edge is one character and a path from the root spells a word. Answers exact-match and **all-words-with-prefix** queries in time proportional to the prefix length, not the dictionary size.

## Intuition

- Shared prefixes share nodes: "foo", "fool", "foolish" branch at one shared `f-o-o` spine -- that sharing is both the memory saving and the query power.
- A word exists iff walking its characters from the root never hits a missing child *and* the final node is flagged `isWord` -- the flag separates "fool" (word) from "fooli" (just a prefix of "foolish").
- `find(prefix)`: walk to the prefix node in O(|prefix|), then DFS the subtree to collect every flagged word below.
- insert / delete / walk are O(L) for word length L; space is O(total characters of all words) in the worst case.
- Deletion usually just clears `isWord` -- nodes stay because other words share them; physical pruning is an optional optimization.

## Approach -- array children (lowercase a-z)

```cpp
using namespace std;

struct Node {
    Node* child[26] = {};     // all nullptr
    bool isWord = false;
};

void insert(Node* root, const string& w) {
    Node* n = root;
    for (char c : w) {
        int i = c - 'a';
        if (!n->child[i]) n->child[i] = new Node();
        n = n->child[i];
    }
    n->isWord = true;
}

void remove(Node* root, const string& w) {
    Node* n = root;
    for (char c : w) {
        n = n->child[c - 'a'];
        if (!n) return;                       // word was never inserted
    }
    n->isWord = false;                        // unmark; shared nodes stay
}

void collect(Node* n, string& prefix, vector<string>& out) {
    if (n->isWord) out.push_back(prefix);
    for (int i = 0; i < 26; ++i)
        if (n->child[i]) {
            prefix.push_back('a' + i);
            collect(n->child[i], prefix, out);
            prefix.pop_back();                // backtrack
        }
}

vector<string> find(Node* root, const string& prefix) {
    Node* n = root;
    for (char c : prefix) {
        n = n->child[c - 'a'];
        if (!n) return {};                    // prefix absent -> no matches
    }
    vector<string> out;
    string p = prefix;
    collect(n, p, out);                       // DFS gathers everything below
    return out;
}
```

- Iterating `child[0..25]` in index order makes `collect` emit words in sorted order for free.
- `collect` mutates one shared `prefix` string with push/pop backtracking -- no per-recursion string copies.
- Pitfall: `find` must check for a missing child at *every* character of the prefix, including the last.
- Pitfall (C++): `delete` is a keyword -- the interface op is conventionally named `remove` or `erase` in code.

## Alternative -- hash-map children

- `unordered_map<char, Node*>` per node instead of `child[26]`: memory proportional to actual branching, and handles arbitrary alphabets (Unicode, tokens as edges).
- Array children: faster (direct index, no hashing) and naturally ordered, but 26 pointers of overhead per node even when sparse.
- Rule of thumb: small fixed alphabet -> array; large or sparse alphabet -> map.

## Alternative -- compressed trie / radix tree

- Collapse single-child chains into one edge labeled with a string; the same queries in O(|word|) but far fewer nodes.
- Best when the dictionary has many short, isolated words with little sharing.

## Complexity

- Time: O(L) per insert / find / remove where L is the word length; O(L + output) for prefix enumeration.
- Space: O(total characters of all words) in the worst case.

## Usage

- Autocomplete / search-as-you-type: `find(prefix)` is exactly the query.
- Spell checkers and word games (Boggle is trie + DFS on a grid).
- IP routing: longest-prefix match is a *bitwise* trie walk over the address.
- T9 / phone-keypad text prediction, dictionary compression.

## Cousins & contrasts

- **Radix / Patricia tree**: compresses single-child chains into one edge labeled with a string -- same queries, far fewer nodes for sparse tries.
- **Suffix tree**: a (compressed) trie of all suffixes of one string; answers "does substring x occur in s" in O(|x|) -- different problem, same idea.
- **Hash set of words**: O(L) exact match like a trie, but can't answer prefix queries at all -- that power is the trie's reason to exist.
- **DAWG / minimal acyclic automaton**: also merges *suffixes*, not just prefixes -- minimal state, harder to build.
